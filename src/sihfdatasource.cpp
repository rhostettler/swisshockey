#include <QDateTime>

#include "sihfdatasource.h"
#include "logger.h"

SIHFDataSource::SIHFDataSource(QObject *parent) : DataSource(parent) {
    // Create the network access objects
    this->nam = new QNetworkAccessManager(this);
    this->decoder = new Json(this);

    // Send an initial query
    //this->gameId = NULL;
    //this->queryScores();
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
/*
   curl 'http://data.sihf.ch/Statistic/api/cms/table?alias=today&size=today&searchQuery=1,2,8,10,11//1,2,4,5,6,7,8,9,20,47,48,49,50,81,90&filterQuery=&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&language=de'
   -H 'Host: data.sihf.ch' -H 'Accept-Encoding: deflate' -H 'Referer: http://www.sihf.ch/de/game-center/' -H 'Connection: keep-alive'
*/
void SIHFDataSource::queryScores(void) {
    // Request URL / Headers
    QString url = "http://data.sihf.ch/Statistic/api/cms/table?alias=today&size=today&searchQuery=1,2,8,10,11//1,2,90&filterQuery=&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&skip=0&language=de";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept-Encoding", "deflate");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Referer", "http://www.sihf.ch/de/game-center/");

    // Send the request and connect the finished() signal of the reply to parser
    this->totomatReply = this->nam->get(request);
    connect(this->totomatReply, SIGNAL(finished()), this, SLOT(parseScoresResponse()));
    connect(this->totomatReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError()));

    // Log the request
    Logger& logger = Logger::getInstance();
    logger.log(Logger::INFO, "Score query sent to server.");
}

// Parse the response from the HTTP Request
void SIHFDataSource::parseScoresResponse() {
    // Get all the raw data
    QByteArray rawdata = this->totomatReply->readAll();

    // Log the raw data for debugging
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "Score query response received, data is:");
    logger.log(Logger::DEBUG, rawdata);

    // Parse the response
    QVariantMap parsedRawdata = this->decoder->decode(rawdata.data());
#if 0
    QMapIterator<QString, QVariant> iter(parsedRawdata);
    while(iter.hasNext()) {
        iter.next();
        logger.log(Logger::DEBUG, iter.key() + " : " + iter.value().typeName());
    }
#endif

    if(parsedRawdata.contains("data")) {
        QVariantList data = parsedRawdata.value("data").toList();
        QListIterator<QVariant> iter(data);
        while(iter.hasNext()) {
            QVariantMap gamedata = this->parseGameSummary(iter.next().toList());

            // TODO: Update the GameDayData according to the old scheme, here, we'll fire a signal instead in the future.
            if(gamedata.size() > 0) {
                this->nlaData->updateGames("", gamedata);
                logger.log(Logger::DEBUG, "Now, there are " + QString(this->nlaData->rowCount()) + " items in the list.");
            } else {
                // NOP?
            }
        }
     }

#if 0
    QMapIterator<QString, QVariant> iter(parsedRawdata);
    while(iter.hasNext()) {
        iter.next();
        logger.log(Logger::DEBUG, iter.key());
    }


    QVariantMap data = this->decoder->decode(this->totomatReply->readAll().data());
    QVariantMap content = data["content"].toMap();
    QVariantMap games = content["games"].toMap();
    QVariantMap dates = content["nav"].toMap();
    QVariantList nla = games["nla"].toList();
    this->nlaData->updateGames(dates["nla"].toString(), nla);

    // Determine when we should update the next time and restart the timer
    //int nextUpdate = this->calculateUpdateInterval(dates["nla"].toString());
    //timer->start(nextUpdate);
#endif
}

QVariantMap SIHFDataSource::parseGameSummary(QVariantList indata) {
    QVariantMap data;

    Logger& logger = Logger::getInstance();

    if(indata.size() == 9) {
        // Extract all the fields for "easy" access
        QString league = indata[0].toString();
        QString time = indata[1].toString();
        QVariantMap hometeam = indata[2].toMap();
        QVariantMap awayteam = indata[3].toMap();
        QVariantMap score = indata[4].toMap();
        QVariantMap periodsScore = indata[5].toMap();
        QVariantMap meta = indata[7].toMap();
        QVariantMap details = indata[8].toMap();

        // Put everything into a QVariantMap that we'll use as the common
        // internal raw data representation
        //QVariantMap data;
        // Add the basic game info
        data.insert("league", league);
        data.insert("gameId", details.value("gameId"));
        //data.insert("");

        // Add the team info
        data.insert("hometeam", hometeam.value("name"));
        data.insert("homeId", hometeam.value("id"));
        data.insert("awayteam", awayteam.value("name"));
        data.insert("awayId", awayteam.value("id"));

        // Put together the score
        QVariantMap scoreArray;
        /*score.insert("first");
        score.insert("second");
        score.insert("third");
        score.insert("overtime");*/
        scoreArray.insert("total", QString(score.value("homeTeam").toString() + ":" + score.value("awayTeam").toString()));
        data.insert("score", scoreArray);

        // Additional info
        // progress, etc.

/*        logger.log(Logger::DEBUG, data[0].toString());
        logger.log(Logger::DEBUG, data[1].toString());
        // Iterate over all fields (?)
        QListIterator<QVariant> iter(data);
        while(iter.hasNext()) {
            QVariant item = iter.next();
            logger.log(Logger::DEBUG, item.typeName());

            //////// dump the rest.
            if(QString::compare(item.typeName(), "QString")) {
                logger.log(Logger::DEBUG, "  - " + item.toString());

            } else if(QString::compare(item.typeName(), "QVariantMap")) {
                QMapIterator<QString, QVariant> iter2(item.toMap());

                while(iter2.hasNext()) {
                    iter2.next();
                    logger.log(Logger::DEBUG, "  - " + iter2.key() + " : " + iter2.value().toString());
                }
            }
        }*/
    } else {
        logger.log(Logger::DEBUG, "It appears that the supplied data doesn't contain any game info.");
    }

    return data;
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
/*    QVariantMap content = data["content"].toMap();

    // If a game for the details was set, update it too
    if(this->gameId != NULL) {
        GameData *game = nlaData->getGame(this->gameId);
        game->updateEvents(content["selected"].toMap(),
                           content["goals"].toList(),
                           content["fouls"].toList(),
                           content["players"].toMap());
    }*/
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
    // Query the website and update
    this->queryScores();
    //this->queryStats();  % Disabled until re-implemented
}
