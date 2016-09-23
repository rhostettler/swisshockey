#include <QDateTime>
#include <cmath>

#include "sihfdatasource.h"
#include "logger.h"

SIHFDataSource::SIHFDataSource(QObject *parent) : DataSource(parent) {
    // Create the network access objects
    this->nam = new QNetworkAccessManager(this);
    this->decoder = new JsonDecoder(this);
}

// TODO: The updateGameDetails() should take an argument which should make this
// unnecessary. The data would be passed to the data store in the same way as the
// game summaries.
#if 0
void SIHFDataSource::setGameId(QString gameId) {
    this->gameId = gameId;
}
#endif

// Send a query to the National League Server
/*
   curl 'http://data.sihf.ch/Statistic/api/cms/table?alias=today&size=today&searchQuery=1,2,8,10,11//1,2,4,5,6,7,8,9,20,47,48,49,50,81,90&filterQuery=&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&language=de'
   -H 'Host: data.sihf.ch' -H 'Accept-Encoding: deflate' -H 'Referer: http://www.sihf.ch/de/game-center/' -H 'Connection: keep-alive'
*/
void SIHFDataSource::queryScores(void) {
    // Notify that the update is being started
    emit updateStarted();

    // Request URL / Headers
    QString url = "http://data.sihf.ch/Statistic/api/cms/table?alias=today&size=today&searchQuery=1,2,8,10,11//1,2,81,90&filterQuery=&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&skip=0&language=de";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept-Encoding", "deflate");
#if 0
    request.setRawHeader("Connection", "keep-alive");
#endif
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
    QVariantMap parsedRawdata = this->decoder->decode(rawdata);
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

    // Signal that we're done parsing the data.
    emit updateFinished();
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

        // Put everything into a QVariantMap that we'll use as the common
        // internal raw data representation
        // Add the basic game info
        data.insert("league", league);
        data.insert("gameId", details.value("gameId"));

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
            if(!statustext.compare("Shootout")) {
                status = 8;
            } else if(!statustext.compare("Ende")) {
                // Note to self: Here we ignore the additional info about the way the game finished (OT/SO)
                // TODO: We should include this nevertheless since the shootout GWG is added to the OT score.
                status = 12;
            } else if(!statustext.compare("Ende*")) {
                if(!otIndicator.compare("OT")) {
                    // Unofficial final result after overtime
                    status = 10;
                } else if(!otIndicator.compare("SO")) {
                    // Unofficial final result after shootout
                    status = 11;
                } else {
                    // Unofficial final result
                    status = 9;
                }
            } else {
                // "End of third", seems to happen from time to time.
                status = 6;
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
    } else if(indata.size() == 1) {
        logger.log(Logger::DEBUG, "SIHFDataSource::parseGameSummary(): It appears that the supplied data doesn't contain any game info (no games today?).");
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseGameSummary(): Something is wrong with the game summary data, maybe a change in the data format?");
    }

    return data;
}

// Query the NL servers for the game stats
/*
    curl 'http://data.sihf.ch/statistic/api/cms/gameoverview?alias=gameDetail&searchQuery=20161105071221&language=de'
    -H 'Accept-Encoding: deflate' -H 'Host: data.sihf.ch' -H 'Referer: http://www.sihf.ch/de/game-center/game/'
 */
void SIHFDataSource::queryStats(QString gameId) {
    // STATUS:
    //  * Request should be OK
    //  * updateStarted() signal missing
    // Request URL / Headers
    QString url = "http://data.sihf.ch/statistic/api/cms/gameoverview?alias=gameDetail&language=de&searchQuery=";
    url.append(gameId);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept-Encoding", "deflate");
    request.setRawHeader("Referer", "http://www.sihf.ch/de/game-center/game/");
    request.setRawHeader("Host", "data.sihf.ch");

    // Send the request and connect the finished() signal of the reply to parser
    this->statsReply = this->nam->get(request);
    connect(statsReply, SIGNAL(finished()), this, SLOT(parseStatsResponse()));
    connect(statsReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError()));
}

// TODO: There's still major work to be done regarding how we should handle
// the game events. As it is now, having three separate lists seems unpractical.
void SIHFDataSource::parseStatsResponse(void) {
    // Get all the raw data
    QByteArray rawdata = this->statsReply->readAll();

    // Log the raw data for debugging
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "SIHFDataSource::parseStatsResponse(): Received the following raw data:");
    logger.log(Logger::DEBUG, rawdata);

    // The API is inconsitent: Apparently, if a game hasn't started, they
    // automatically include the callback function so we have to strip that
    // before we can proceed
    QByteArray callbackString("externalStatisticsCallback(");
    if(rawdata.startsWith(callbackString)) {
        // Remove the callback name and the trailing ");"
        rawdata.remove(0, callbackString.length());
        rawdata.chop(2);

        logger.log(Logger::DEBUG, "SIFHDataSource::parseStatsResponse(): Removed callback string:");
        logger.log(Logger::DEBUG, rawdata);
    }

    // Convert from JSON to a map
    QVariantMap parsedRawdata = this->decoder->decode(rawdata);

    // Extract the game events
    QVariantList goals;
    QVariantList fouls;
    QVariantList goalkeepers; // TODO: Not implemented yet.
    if(parsedRawdata.contains("summary")) {
        QVariantMap summary = parsedRawdata["summary"].toMap();
        QVariantList periods = summary["periods"].toList();
        QListIterator<QVariant> iter(periods);
        while(iter.hasNext()) {
            QVariantMap period = iter.next().toMap();
            goals.append(period["goals"].toList());
            fouls.append(period["fouls"].toList());
        }
        logger.log(Logger::DEBUG, "SIHFDataSource::parseStatsResponse(): Number of goals: " + QString::number(goals.size()));
        logger.log(Logger::DEBUG, "SIHFDataSource::parseStatsResponse(): Number of fouls: " + QString::number(fouls.size()));
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseStatsResponse(): No game events data found!");
    }

    // Extract the players
    QVariantList players;
    if(parsedRawdata.contains("players")) {
        players.append(parsedRawdata["players"].toList());
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseStatsResponse(): No player data found!");
    }

    // If a game for the details was set, update it too
    emit gameDetailsUpdated(goals, fouls, players);
}

// Update the data from this source
// TODO: Implement so that we either can update the summaries or the details
void SIHFDataSource::update(QString id) {
    // Query the website and update
    this->queryScores();

    // TODO: Doesn't update the stats periodically yet
    if(id != NULL) {
        this->queryStats(id);
    }
}

// Handle possible errors when sending queries over the network
void SIHFDataSource::handleNetworkError(QNetworkReply::NetworkError error) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::ERROR, "SIHFDataSource::handleNetworkError(): Network error occured.");
}
