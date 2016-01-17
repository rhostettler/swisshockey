#include <QDateTime>
#include <cmath>

#include "sihfdatasource.h"
#include "logger.h"

SIHFDataSource::SIHFDataSource(QObject *parent) : DataSource(parent) {
    // Create the network access objects
    this->nam = new QNetworkAccessManager(this);
    this->decoder = new Json(this);
}

// TODO: The updateGameDetails() should take an argument which should make this
// unnecessary. The data would be passed to the data store in the same way as the
// game summaries.
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
    QString url = "http://data.sihf.ch/Statistic/api/cms/table?alias=today&size=today&searchQuery=1,2,8,10,11//1,2,81,90&filterQuery=&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&skip=0&language=de";
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
    logger.log(Logger::INFO, "SIHFDataSource::queryScores(): Query sent to server.");
}

// Parse the response from the HTTP Request
void SIHFDataSource::parseScoresResponse() {
    // Get all the raw data
    QByteArray rawdata = this->totomatReply->readAll();

    // Log the raw data for debugging
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "SIHFDataSource::parseScoresResponse(): Received the following raw data:");
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

    // If the 'data'-field exists, we try to parse it
    if(parsedRawdata.contains("data")) {
        logger.log(Logger::DEBUG, "SIHFDataSource::parseScoresResponse(): Parsing data...");
        QVariantList data = parsedRawdata.value("data").toList();
        QListIterator<QVariant> iter(data);
        while(iter.hasNext()) {
            QVariantMap gamedata = this->parseGameSummary(iter.next().toList());

            if(gamedata.size() > 0) {
                // Signal that we have new data to consider
                emit gameSummaryUpdated(gamedata);
            } else {
                // NOP?
            }
        }
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseScoresResponse(): No 'data' field in the response from the server.");
    }
}

// Parse the per-game JSON array from the response and put everything in an
// associative array with predefined fields for internal data exchange between
// data sources and data stores.
QVariantMap SIHFDataSource::parseGameSummary(QVariantList indata) {
    QVariantMap data;

    Logger& logger = Logger::getInstance();

    if(indata.size() == 9) {
        // Extract all the fields for "easy" access
        QString league = indata[0].toString();
        QString time = indata[1].toString();
        QVariantMap hometeam = indata[2].toMap();
        QVariantMap awayteam = indata[3].toMap();
        QVariantMap totalScore = indata[4].toMap();
        QVariantMap periodsScore = indata[5].toMap();
        QString otIndicator = indata[6].toString();
        QVariantMap meta = indata[7].toMap();
        QVariantMap details = indata[8].toMap();

        // TODO: This is just a quick & dirty workaround right now to only
        // fetch the NLA data, this must be handled properly using a filter in
        // the presentation layer in the future
        if(!QString::compare(league, "NL A") || !QString::compare(league, "Cup") || !QString::compare(league, "CHL")) {
            // Put everything into a QVariantMap that we'll use as the common
            // internal raw data representation
            //QVariantMap data;
            // Add the basic game info
            data.insert("league", league);
            data.insert("gameId", details.value("gameId"));
            //data.insert("");

            // Add the team info
            data.insert("hometeam", hometeam.value("name"));
            data.insert("hometeamId", hometeam.value("id"));
            data.insert("awayteam", awayteam.value("name"));
            data.insert("awayteamId", awayteam.value("id"));

            // Put together the score
            QVariantList homePeriodsScore = periodsScore.value("homeTeam").toList();
            QVariantList awayPeriodsScore = periodsScore.value("awayTeam").toList();
            QVariantMap score;
            score["first"] = homePeriodsScore.value(0, "-").toString() + ":" + awayPeriodsScore.value(0, "-").toString();
            score["second"] = homePeriodsScore.value(1, "-").toString() + ":" + awayPeriodsScore.value(1, "-").toString();
            score["third"] = homePeriodsScore.value(2, "-").toString() + ":" + awayPeriodsScore.value(2, "-").toString();
            if(homePeriodsScore.size() == 4) {
                score.insert("overtime", homePeriodsScore[3].toString() + ":" + awayPeriodsScore[3].toString());
            }
            score["total"] = totalScore.value("homeTeam").toString() + ":" + totalScore.value("awayTeam").toString();
            data.insert("score", score);

            // Additional info, progress, etc.
            // 0 - Not started
            // 17 - 1. period
            // 33 - 1. break
            // 50 - 2. Period
            // 67 - 2. break
            // 83 - 3. Period
            // 88 + "Overtime"
            // 100 - Finished
            // 100 + "Shootout"
            // 100 + "Ende*"
            // 100 + "Ende"
            // Roughly corresponds to the following formula: progress/100*6 = "old status code"
            double progress = meta.value("percent").toDouble();
            int status = 0;
            if(progress == 100) {
                QString statustext = meta.value("name").toString();
                if(statustext.compare("Shootout")) {
                    status = 8;
                } else if(statustext.compare("Ende")) {
                    // Note to self: Here we ignore the additional info about the way the game finished (OT/SO)
                    status = 12;
                } else if(statustext.compare("Ende*")) {
                    if(otIndicator.compare("OT")) {
                        // Unofficial final result after overtime
                        status = 10;
                    } else if(otIndicator.compare("SO")) {
                        // Unofficial final result after shootout
                        status = 11;
                    } else {
                        // Unofficial final result
                        status = 9;
                    }
                }
            } else if(progress == 88) {
                // Overtime in progress
                status = 7;
            } else {
                // Regular, 1, ..., 6
                status = round(progress/100*6);
            }
            data.insert("status", status);
            logger.log(Logger::DEBUG, "SIHFDataSource::parseGameSummary(): Game status calculated to be " + data.value("status").toString());
        } else {
            logger.log(Logger::DEBUG, "SIHFDataSource::parseGameSummary(): Not NLA, Cup, or CHL, data discarded.");
        }
    } else if(indata.size() == 1) {
        logger.log(Logger::DEBUG, "SIHFDataSource::parseGameSummary(): It appears that the supplied data doesn't contain any game info (no games today?).");
        // TODO: here, we should somehow signal that there are no games today? Or maybe that's to be done somewhere else instead?
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseGameSummary(): Something is wrong with the game summary data, maybe a change in the data format?");
    }

    return data;
}

// Query the NL servers for the game stats
// TODO: Re-implement completely!
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

// TODO: Re-implement completely
void SIHFDataSource::parseStatsResponse(void) {
#if 0
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
#endif
}

// Update the data from this source
// TODO: Implement so that we either can update the summaries or the details
void SIHFDataSource::update() {
    // Query the website and update
    this->queryScores();
    //this->queryStats();  % Disabled until re-implemented
}

// Handle possible errors when sending queries over the network
void SIHFDataSource::handleNetworkError(QNetworkReply::NetworkError error) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::ERROR, "SIHFDataSource::handleNetworkError(): Network error occured.");
}
