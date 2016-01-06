#include <QDateTime>
#include "sihfdatasource.h"

SIHFDataSource::SIHFDataSource(QObject *parent) : DataSource(parent) {
    // Create the network access objects
    this->nam = new QNetworkAccessManager(this);
    this->decoder = new Json(this);

    // Send an initial query
    //this->gameId = NULL;
    this->queryScores();
}

// Set the data store
void SIHFDataSource::setData(GamedayData *data){
    // TODO: should we discard a possible old list first? better be safe...
    this->nlaData = data;
}

// Return the data store
GamedayData* SIHFDataSource::getData() {
    return this->nlaData;
}

void SIHFDataSource::setGameId(QString gameId) {
    this->gameId = gameId;
}

// Send a query to the National League Server
void SIHFDataSource::queryScores(void) {
    // Request URL / Headers
    QString url = "http://data.sihf.ch/Statistic/api/cms/table?alias=today&searchQuery=1&filterQuery=1&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&size=today";

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept-Encoding", "deflate");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Referer", "http://www.sihf.ch/de/game-center/");

    // Request parameters: None
    QByteArray parameters;

    // Send the request and connect the finished() signal of the reply to parser
    this->totomatReply = this->nam->post(request, parameters);
    connect(totomatReply, SIGNAL(finished()), this, SLOT(parseSIHFDataSourceResponse()));
    connect(totomatReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError()));
}

// Parse the response from the HTTP Request
void SIHFDataSource::parseScoresResponse() {
    // Extract the essential part of the response and pass it to the data list
    QVariantMap data = this->decoder->decode(this->totomatReply->readAll().data());
    QVariantMap content = data["content"].toMap();
    QVariantMap games = content["games"].toMap();
    QVariantMap dates = content["nav"].toMap();
    QVariantList nla = games["nla"].toList();
    this->nlaData->updateGames(dates["nla"].toString(), nla);

    // Determine when we should update the next time and restart the timer
    //int nextUpdate = this->calculateUpdateInterval(dates["nla"].toString());
    //timer->start(nextUpdate);
}

// Query the NL servers for the game stats
void SIHFDataSource::queryStats(void) {
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
        connect(statsReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError()));
    }
}

// Handle possible errors when querying the SIHFDataSource
void SIHFDataSource::handleNetworkError(QNetworkReply::NetworkError error) {
    // Set the timer to try again in 10 seconds
    /*this->timer->stop();
    this->timer->start(10000);*/
}

void SIHFDataSource::parseStatsResponse(void) {
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
qint64 SIHFDataSource::calculateUpdateInterval(QString date) {
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

// Update the SIHFDataSource when the timer times out
void SIHFDataSource::update() {
    // First, disable the timer
    //timer->stop();

    // Query the website and update
    this->queryScores();
    //this->queryStats();  % Disabled until re-implemented
}
