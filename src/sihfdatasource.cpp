#include <cmath>

#include "sihfdatasource.h"
#include "logger.h"
#include "league.h"

// TODO: I should only store the base URLs and then add the parameters dynamically. In particular, this would be helpful if the baseurl changes
const QString SIHFDataSource::SCORES_URL = "http://data.sihf.ch/Statistic/api/cms/table?alias=today&size=today&searchQuery=1,2,8,10,11//1,2,8,81,90&filterQuery=&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&skip=0&language=de";
const QString SIHFDataSource::DETAILS_URL = "http://data.sihf.ch/statistic/api/cms/gameoverview?alias=gameDetail&language=de&searchQuery=";

SIHFDataSource::SIHFDataSource(QObject *parent) : DataSource(parent) {
    // Create the network access objects
    this->nam = new QNetworkAccessManager(this);
    this->decoder = new JsonDecoder(this);
}

// Update the game summaries
void SIHFDataSource::getGameSummaries(void) {
    // Notify that the update is being started
    emit updateStarted();

    // Request URL and headers
    QNetworkRequest request;
    request.setUrl(QUrl(SIHFDataSource::SCORES_URL));
    request.setRawHeader("Accept-Encoding", "deflate");
    request.setRawHeader("Referer", "http://www.sihf.ch/de/game-center/");

    // Send the request and connect the finished() signal of the reply to parser
    this->summariesReply = this->nam->get(request);
    connect(this->summariesReply, SIGNAL(finished()), this, SLOT(parseGameSummaries()));
    connect(this->summariesReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError(QNetworkReply::NetworkError)));

    // Log the request
    Logger& logger = Logger::getInstance();
    logger.log(Logger::INFO, "SIHFDataSource::queryScores(): Query sent to server.");
}

// Parse the response from the HTTP Request
void SIHFDataSource::parseGameSummaries() {
    // Get the raw data
    QByteArray rawdata = this->summariesReply->readAll();

    // Log the raw data for debugging
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "SIHFDataSource::parseSummariesResponse(): Received the following raw data:");
    logger.log(Logger::DEBUG, rawdata);

    // Parse the response
    QVariantMap parsedRawdata = this->decoder->decode(rawdata);
    if(parsedRawdata.contains("data")) {
        logger.log(Logger::DEBUG, "SIHFDataSource::parseSummariesResponse(): Parsing data...");
        QVariantList data = parsedRawdata.value("data").toList();
        QListIterator<QVariant> iter(data);
        while(iter.hasNext()) {
            QVariantMap gamedata = this->parseGame(iter.next().toList());

            if(gamedata.size() > 0) {
                emit summaryUpdated(gamedata);
            } else {
                // NOP?
            }
        }
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseSummariesResponse(): No 'data' field in the response from the server.");
    }

    emit updateFinished();
}

// Parse the per-game JSON array from the response and put everything in an
// associative array with predefined fields for internal data exchange between
// data sources and data stores.
QVariantMap SIHFDataSource::parseGame(QVariantList indata) {
    QVariantMap data;

    Logger& logger = Logger::getInstance();

    // TODO: Move the constant (10) to the header
    if(indata.size() == 10) {
        // Extract all the fields for "easy" access
        QString league = indata[0].toString();
        QString time = indata[1].toString();
        QVariantMap hometeam = indata[2].toMap();
        QVariantMap awayteam = indata[3].toMap();
        // 4 is 'Phase', e.g. regular season, etc.
        QVariantMap totalScore = indata[5].toMap();
        QVariantMap periodsScore = indata[6].toMap();
        QString otIndicator = indata[7].toString();
        QVariantMap meta = indata[8].toMap();
        QVariantMap details = indata[9].toMap();

        // Put everything into a QVariantMap that we'll use as the common
        // internal raw data representation
        // Add the basic game info
        data.insert("league", SIHFDataSource::getLeagueId(league));
        data.insert("time", time);
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
void SIHFDataSource::getGameDetails(QString gameId) {
    // TODO: Uses the same signal as getGameSummaries(), might consider using its
    emit updateStarted();

    // Request URL and eaders
    QNetworkRequest request;
    request.setUrl(QUrl(SIHFDataSource::DETAILS_URL + gameId));
    request.setRawHeader("Accept-Encoding", "deflate");
    request.setRawHeader("Referer", "http://www.sihf.ch/de/game-center/game/");
    //request.setRawHeader("Host", "data.sihf.ch");

    // Send the request and connect the finished() signal of the reply to parser
    this->detailsReply = this->nam->get(request);
    connect(detailsReply, SIGNAL(finished()), this, SLOT(parseGameDetails()));
    connect(detailsReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError()));
}

// Parse the response of a getGameDetails() request
void SIHFDataSource::parseGameDetails(void) {
    // Get the raw data
    QByteArray rawdata = this->detailsReply->readAll();

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

    // Convert from JSON to a map, then parse the game details
    QVariantMap parsedRawdata = this->decoder->decode(rawdata);

    // Parse all the players; this is done before parsing the events to make
    // that the players can be found when the events are rendered in the UI
    // TODO: We might want to think about how we handle the players in the future
    //QList<Player> players;
    QVariantList players;
    if(parsedRawdata.contains("players")) {
        players.append(parsedRawdata["players"].toList());
        emit playersUpdated(players);
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseStatsResponse(): No player data found!");
    }

    // Parse all the game events
    QList<GameEvent *> events;
    if(parsedRawdata.contains("summary")) {
        QVariantMap summary = parsedRawdata["summary"].toMap();
        QVariantList periods = summary["periods"].toList();
        QListIterator<QVariant> iter(periods);
        while(iter.hasNext()) {
            QVariantMap period = iter.next().toMap();
            events.append(this->parseGoals(period["goals"].toList()));
            events.append(this->parsePenalties(period["fouls"].toList()));
            events.append(this->parseGoalkeepers(period["goalkeepers"].toList()));
        }
        QVariantMap shootout = summary["shootout"].toMap();
        events.append(this->parseShootout(shootout["shoots"].toList()));
        logger.log(Logger::DEBUG, "SIHFDataSource::parseStatsResponse(): Number of parsed events: " + QString::number(events.size()));
    } else {
        logger.log(Logger::ERROR, "SIHFDataSource::parseStatsResponse(): No game events data found!");
    }
    emit eventsUpdated(events);

}

// Parses the goals data and returns an unsorted QList<GameEvent>
QList<GameEvent *> SIHFDataSource::parseGoals(QVariantList data) {
    QList<GameEvent *> events;
    QListIterator<QVariant> iterator(data);
    while(iterator.hasNext()) {
        QVariantMap goal = iterator.next().toMap();
        GameEvent *event = new GameEvent(GameEvent::GOAL);
        event->setTime(goal.value("time").toString());
        event->setTeam(goal.value("teamId").toUInt());

        // Parse the goal text to extract the score and play (PP1 / EQ / etc.).
        // The format is '**EQ,GWG** / **0:1** - <Player Names>'.
        QString haystack = goal.value("text").toString();
        QRegExp typeNeedle("(\\w+)");
        typeNeedle.indexIn(haystack);
        QString type = typeNeedle.cap(1);
        QRegExp scoreNeedle("(\\d+:\\d+)");
        scoreNeedle.indexIn(haystack);
        QString score = scoreNeedle.cap(1);
        event->setScore(score, type);
        event->addPlayer(GameEvent::SCORER, goal.value("scorerLicenceNr").toUInt());
        event->addPlayer(GameEvent::FIRST_ASSIST, goal.value("assist1LicenceNr").toUInt());
        event->addPlayer(GameEvent::SECOND_ASSIST, goal.value("assist2LicenceNr").toUInt());
        events.append(event);
    }

    return events;
}

// Parses the penalties data and returns an unsorted QList<GameEvent>
QList<GameEvent *> SIHFDataSource::parsePenalties(QVariantList data) {
    QList<GameEvent *> events;
    QListIterator<QVariant> iterator(data);
    while(iterator.hasNext()) {
        QVariantMap penalty = iterator.next().toMap();
        GameEvent *event = new GameEvent(GameEvent::PENALTY);
        event->setTime(penalty.value("time").toString());
        event->setTeam(penalty.value("teamId").toLongLong());
        event->addPlayer(GameEvent::PENALIZED, penalty.value("playerLicenceNr").toUInt());
        event->setPenalty(penalty.value("id").toInt(), penalty.value("minutes").toString() + "'");
        events.append(event);
    }

    return events;
}

// Parses the GK events
QList<GameEvent *> SIHFDataSource::parseGoalkeepers(QVariantList data) {
    QList<GameEvent *> events;
    QListIterator<QVariant> iterator(data);
    while(iterator.hasNext()) {
        QVariantMap tmp = iterator.next().toMap();
        // Parse the action from the human readable data since it isn't provided in the data
        // The format is '<Player Name> (ACTION)'. where ACTION is either IN or OUT
        int type = GameEvent::GOALKEEPER_OUT;
        QString haystack = tmp.value("text").toString();
        QRegExp needle("\\(IN\\)");
        if(needle.indexIn(haystack) != -1) {
            type = GameEvent::GOALKEEPER_IN;
        }
        GameEvent *event = new GameEvent(type);
        event->setTime(tmp.value("time").toString());
        event->setTeam(tmp.value("teamId").toLongLong());
        event->addPlayer(GameEvent::GOALKEEPER, tmp.value("playerLicenceNr").toUInt());
        events.append(event);
    }

    return events;
}

// Parse shootout
QList<GameEvent *> SIHFDataSource::parseShootout(QVariantList data) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "SIHFDataSource:parseShootout(): Parsing shootout, " + QString::number(data.size()) + " shots.");

    QList<GameEvent *> events;
    QListIterator<QVariant> iterator(data);
    while(iterator.hasNext()) {
        QVariantMap tmp = iterator.next().toMap();
        GameEvent *event = new GameEvent(GameEvent::PENALTY_SHOT);
        event->setTime("65:00." + tmp["number"].toString());
        event->setPenaltyShot(tmp["scored"].toBool());
        //event->setTeam(); <- Team is not set here :(
        event->addPlayer(GameEvent::SCORER, tmp["scorerLicenceNr"].toUInt());
        event->addPlayer(GameEvent::GOALKEEPER, tmp["goalkeeperLiceneNr"].toUInt());
        events.append(event);
    }

    return events;
}

// Update the data from this source
void SIHFDataSource::update(QString id) {
    // Query the website and update
    this->getGameSummaries();
    if(id != NULL) {
        this->getGameDetails(id);
    }
}

// Handle possible errors when sending queries over the network
void SIHFDataSource::handleNetworkError(QNetworkReply::NetworkError error) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::ERROR, "SIHFDataSource::handleNetworkError(): Network error occured.");
}

void SIHFDataSource::getLeagues(QList<QObject *> *leagueList) {
    // Populate the list of leagues (static for this data source)
    QMapIterator<uint, League *> iLeague(mLeaguesMap);
    while(iLeague.hasNext()) {
        leagueList->append(iLeague.next().value());
    }
}

// League list initialization and access
QMap<uint, League *> SIHFDataSource::mLeaguesMap = initLeagueList();
const QMap<uint, League *> SIHFDataSource::initLeagueList() {
    QMap<uint, League *> map;
    map.insert(0, new League(0, "All", "All"));
    map.insert(1, new League(1, "NL", "National League"));
    map.insert(2, new League(2, "SL", "Swiss League"));
    map.insert(8, new League(8, "Länderspiel A", "Men's National Team")); // TODO: What's the correct key for this (i.e. "Länderspiel A")
    map.insert(89, new League(89, "SIHC", "Cup"));
    map.insert(90, new League(90, "CHL", "CHL"));
    return map;
}

// Retrieves the league ID from the list of leagues
QString SIHFDataSource::getLeagueId(QString abbreviation) {
    bool found = false;
    QString id = "-1";
    QMapIterator<uint, League *> iLeague(mLeaguesMap);
    while(iLeague.hasNext() && !found) {
        League *league = iLeague.next().value();
        if(abbreviation.compare(league->getAbbreviation()) == 0) {
            id = league->getId();
            found = true;
        }
    }
    return id;
}
