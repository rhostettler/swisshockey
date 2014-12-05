#include <QDateTime>
#include "totomat.h"

Totomat::Totomat(QObject *parent) : QObject(parent) {
    // Create the network access objects
    this->nam = new QNetworkAccessManager(this);
    this->decoder = new Json(this);

    // Create a timer and set the update interval
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(30000);

    // Send an initial query
    //this->gameId = NULL;
    this->queryTotomat();
}

// Set the data store
void Totomat::setData(GamedayData *data){
    // TODO: should we discard a possible old list first? better be safe...
    this->nlaData = data;
}

// Return the data store
GamedayData* Totomat::getData() {
    return this->nlaData;
}

void Totomat::setGameId(QString gameId) {
    this->gameId = gameId;
}

// Send a query to the National League Server
void Totomat::queryTotomat(void) {
    // Request URL / Headers
    QString url = "http://www.swiss-icehockey.ch/liveticker/totomat.php";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
    request.setRawHeader("Connection", "Close");

    // Request parameters: None
    QByteArray parameters;

    // Send the request and connect the finished() signal of the reply to parser
    this->totomatReply = this->nam->post(request, parameters);
    connect(totomatReply, SIGNAL(finished()), this, SLOT(parseTotomatResponse()));
}

// Parse the response from the HTTP Request
void Totomat::parseTotomatResponse() {
    // Extract the essential part of the response and pass it to the data list
    QVariantMap data = this->decoder->decode(this->totomatReply->readAll().data());
    QVariantMap content = data["content"].toMap();
    QVariantMap games = content["games"].toMap();
    QVariantMap dates = content["nav"].toMap();
    QVariantList nla = games["nla"].toList();
    this->nlaData->updateGames(dates["nla"].toString(), nla);

    // Determine when we should update the next time and restart the timer
    int nextUpdate = this->calculateUpdateInterval(dates["nla"].toString());
    timer->start(nextUpdate);
}

// Query the NL servers for the game stats
void Totomat::queryStats(void) {
    if(this->gameId != NULL) {
        // Request URL / Headers
        QString url = "http://www.swiss-icehockey.ch/liveticker/stats.php";
        url.append("?gameid=").append(this->gameId);

        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
        request.setRawHeader("Connection", "Close");

        // Request parameters: None
        QByteArray parameters;

        // Send the request and connect the finished() signal of the reply to parser
        this->statsReply = this->nam->post(request, parameters);
        connect(statsReply, SIGNAL(finished()), this, SLOT(parseStatsResponse()));
    }
}

void Totomat::parseStatsResponse(void) {
    QVariantMap data = this->decoder->decode(this->statsReply->readAll().data());
    QVariantMap content = data["content"].toMap();

    // If a game for the details was set, update it too
    if(this->gameId != NULL) {
        GameData *game = nlaData->getGame(this->gameId);
        game->updateEvents(content["selected"].toMap(),
                           content["goals"].toList(),
                           content["fouls"].toList(),
                           content["players"].toMap());
    }
}

// Calculate the query interval based on the game date such that we can have the
// app open in the background without flooding the liveticker
qint64 Totomat::calculateUpdateInterval(QString date) {
    // Get the date and time now
    QDateTime now = QDateTime::currentDateTime();

    // Determine when to update the next time
    QDateTime gameDateTime = QDateTime::fromString(date, "dd.MM.yyyy");
    switch(gameDateTime.date().dayOfWeek()) {
        // Tuesday, Thursday, Friday, Saturday: Game starts at 19.45 (normally),
        // so we start updating 5 mins before
        case 2:
        case 4:
        case 5:
        case 6:
            gameDateTime.setTime(QTime::fromString("19:40", "hh:mm"));
            break;

        // Sundays, the game starts at 16.00 so we start updating at 15.55
        case 7:
            gameDateTime.setTime(QTime::fromString("15:55", "hh:mm"));
            break;

        // In any other case, something is fishy. Nevertheless, we set it to the
        // current time so that we'll update soon again
        default:
            gameDateTime.setTime(QTime::currentTime());
            break;
    }

    // Determine the time until it's game time (or, if the game has started,
    // set the interval to 30s)
    qint64 interval = now.msecsTo(gameDateTime);
    if(interval <= 0) {
        interval = 30000;
    }

    return interval;
}

// Update the totomat when the timer times out
void Totomat::update() {
    // First, disable the timer
    timer->stop();

    // Query the website and update
    this->queryTotomat();
    this->queryStats();
}
