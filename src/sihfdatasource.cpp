/*
 * Copyright 2014-2017 Roland Hostettler
 *
 * This file is part of swisshockey.
 *
 * swisshockey is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * swisshockey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * swisshockey. If not, see http://www.gnu.org/licenses/.
 */

#include <cmath>

#include "sihfdatasource.h"
#include "logger.h"
#include "league.h"

// TODO: I should only store the base URLs and then add the parameters dynamically. In particular, this would be helpful if the baseurl changes
const QString SIHFDataSource::SCORES_URL = "http://data.sihf.ch/Statistic/api/cms/table?alias=today&size=today&searchQuery=1,2,8,10,11//1,2,8,81,90&filterQuery=&orderBy=gameLeague&orderByDescending=false&take=20&filterBy=League&skip=0&language=de";
const QString SIHFDataSource::DETAILS_URL = "http://data.sihf.ch/statistic/api/cms/gameoverview?alias=gameDetail&language=de&searchQuery=";

SIHFDataSource::SIHFDataSource(GamedayData *gamesList, QObject *parent) : DataSource(gamesList, parent) {
    // Create the network access objects
    mNetworkManager = new QNetworkAccessManager(this);
    mJSONDecoder = new JsonDecoder(this);
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
    mSummariesReply = mNetworkManager->get(request);
    connect(mSummariesReply, SIGNAL(finished()), this, SLOT(parseGameSummaries()));
    connect(mSummariesReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError(QNetworkReply::NetworkError)));

    // Log the request
    Logger& logger = Logger::getInstance();
    logger.log(Logger::INFO, QString(Q_FUNC_INFO).append(": Query sent to server."));
}

// Parse the response from the HTTP Request
void SIHFDataSource::parseGameSummaries() {
    // Get the raw data
    QByteArray rawdata = mSummariesReply->readAll();

    // Log the raw data for debugging
    Logger& logger = Logger::getInstance();
    QString dumpfile("dump-summaries-" + QDateTime::currentDateTime().toString("yyyy-MM-ddTHHmmss") + ".json");
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Dumping response data to " + dumpfile + "."));
    logger.dump(dumpfile, rawdata);

    // Parse the response
    QVariantMap parsedRawdata = this->mJSONDecoder->decode(rawdata);
    if(parsedRawdata.contains("data")) {
        logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Parsing data..."));
        QVariantList data = parsedRawdata.value("data").toList();
        QListIterator<QVariant> iter(data);
        while(iter.hasNext()) {
            parseGame(iter.next().toList());
        }
    } else {
        logger.log(Logger::ERROR, QString(Q_FUNC_INFO).append(": No 'data' field in the response from the server."));
    }

    emit updateFinished();
}

// Parse the per-game JSON array from the response and put everything in an
// associative array with predefined fields for internal data exchange between
// data sources and data stores.
void SIHFDataSource::parseGame(QVariantList data) {
    Logger& logger = Logger::getInstance();

    // Check lenght of game summary data. Swiss league data may be one entry shorter; broadcast field may be missing
    if(data.size() == SIHFDataSource::GS_LENGTH || data.size() == SIHFDataSource::GS_LENGTH-1) {
        // Get game ID
        QVariantMap details = data[SIHFDataSource::GS_DETAILS].toMap();
        QString gameId = details.value("gameId").toString();
        GameData *game = mGamesList->getGame(gameId);

        if(game == NULL) {
            // Create a new game
            game = new GameData(gameId, mGamesList);

            // Set game info; this is static info, so we only do it the first time
            QString league = data[SIHFDataSource::GS_LEAGUE_NAME].toString();
            QString time = data[SIHFDataSource::GS_TIME].toString();
            QVariantMap hometeam = data[SIHFDataSource::GS_HOMETEAM].toMap();
            QVariantMap awayteam = data[SIHFDataSource::GS_AWAYTEAM].toMap();

            // Add the basic game info
            game->setLeague(SIHFDataSource::getLeagueId(league));
            game->setDateTime(time);  // TODO: Convert to QDateTime in UTC

            // Add the team info
            // TODO: This should make use of the new classe "Team" to be created
            game->setHometeam(hometeam.value("id").toString(), hometeam.value("name").toString());
            game->setAwayteam(awayteam.value("id").toString(), awayteam.value("name").toString());

            // TODO: Set infos such as place, attendance, refs, etc. (Attendance could actually be set later on as it might change)

            // Add the game to the games list
            mGamesList->addGame(game);

            // Debugging
//            logger.log(Logger::ERROR, QString(Q_FUNC_INFO).append(": Game ID " + data["gameId"] + " not found."));
        } else {
            // NOP
        }

        // Get score and progress info
        QVariantMap totalScore = data[SIHFDataSource::GS_TOTALSCORE].toMap();
        QVariantMap periodsScore = data[SIHFDataSource::GS_PERIODSSCORE].toMap();
        QString otIndicator = data[SIHFDataSource::GS_OTINDICATOR].toString();
        QVariantMap meta = data[SIHFDataSource::GS_META].toMap();

        // Put together the score
        QVariantList homePeriodsScore = periodsScore.value("homeTeam").toList();
        QVariantList awayPeriodsScore = periodsScore.value("awayTeam").toList();
        QMap<QString, QString> score;
        score["first"] = homePeriodsScore.value(0, "-").toString() + ":" + awayPeriodsScore.value(0, "-").toString();
        score["second"] = homePeriodsScore.value(1, "-").toString() + ":" + awayPeriodsScore.value(1, "-").toString();
        score["third"] = homePeriodsScore.value(2, "-").toString() + ":" + awayPeriodsScore.value(2, "-").toString();
        if(homePeriodsScore.size() == 4) {
            score.insert("overtime", homePeriodsScore[3].toString() + ":" + awayPeriodsScore[3].toString());
        }
        score["total"] = totalScore.value("homeTeam").toString() + ":" + totalScore.value("awayTeam").toString();
        game->setScore(score);

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
        // TODO: Remove those old status hard-coded status codes and replace them by enum-type statuses.
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
        game->setStatus(status);
        logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Game status calculated to be " + status));
    } else if(data.size() == 1) {
        logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": It appears that the supplied data doesn't contain any game info (no games today?)."));
    } else {
        logger.log(Logger::ERROR, QString(Q_FUNC_INFO).append(": Something is wrong with the game summary data, maybe a change in the data format?"));
    }
}

// Query the NL servers for the game stats
void SIHFDataSource::getGameDetails(QString gameId) {
    // TODO: Uses the same signal as getGameSummaries(), might consider using its own
    emit updateStarted();

    // Request URL and eaders
    QNetworkRequest request;
    request.setUrl(QUrl(SIHFDataSource::DETAILS_URL + gameId));
    request.setRawHeader("Accept-Encoding", "deflate");
    request.setRawHeader("Referer", "http://www.sihf.ch/de/game-center/game/");
    //request.setRawHeader("Host", "data.sihf.ch");

    // Send the request and connect the finished() signal of the reply to parser
    mDetailsReply = mNetworkManager->get(request);
    connect(mDetailsReply, SIGNAL(finished()), this, SLOT(parseGameDetails()));
    connect(mDetailsReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleNetworkError(QNetworkReply::NetworkError)));
}

// Parse the response of a getGameDetails() request
void SIHFDataSource::parseGameDetails(void) {
    // Get the raw data
    QByteArray rawdata = mDetailsReply->readAll();

    // The API is inconsitent: Apparently, if a game hasn't started, they
    // automatically include the callback function so we have to strip that
    // before we can proceed
    QByteArray callbackString("externalStatisticsCallback(");
    if(rawdata.startsWith(callbackString)) {
        // Remove the callback name and the trailing ");"
        rawdata.remove(0, callbackString.length());
        rawdata.chop(2);
    }

    // Log the raw data for debugging
    Logger& logger = Logger::getInstance();
    QString dumpfile("dump-details-" + QDateTime::currentDateTime().toString("yyyy-MM-ddTHHmmss") + ".json");
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Dumping details data in " + dumpfile + "."));
    logger.dump(dumpfile, rawdata);

    // Convert from JSON to a map, then parse the game details
    QVariantMap data = mJSONDecoder->decode(rawdata);
    QString gameId = data["gameId"].toString();
    GameData *game = mGamesList->getGame(gameId);
    if(game != NULL) {
        // Parse all the players; this is done before parsing the events to ensure
        // that the players can be found when the events are rendered in the UI
        if(data.contains("players")) {
            parsePlayers(game, data["players"].toList());
        } else {
            logger.log(Logger::ERROR, QString(Q_FUNC_INFO).append(": No player data found!"));
        }

#if 0
        QList<Player *> players;
        if(data.contains("players")) {
            players.append(parsePlayers(data["players"].toList()));

            // TODO: Instead of emitting a signal here, simply add the players to the game
            emit playersUpdated(players);
        } else {
            logger.log(Logger::ERROR, QString(Q_FUNC_INFO).append(": No player data found!"));
        }
#endif

        // Parse all the game events
        QList<GameEvent *> events;
        if(data.contains("summary")) {
            QVariantMap summary = data["summary"].toMap();
            QVariantList periods = summary["periods"].toList();
            QListIterator<QVariant> iter(periods);
            while(iter.hasNext()) {
                QVariantMap period = iter.next().toMap();
                events.append(parseGoals(period["goals"].toList()));
                events.append(parsePenalties(period["fouls"].toList()));
                events.append(parseGoalkeepers(period["goalkeepers"].toList()));
            }
            QVariantMap shootout = summary["shootout"].toMap();
            events.append(parseShootout(shootout["shoots"].toList()));
            logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Number of parsed events: " + QString::number(events.size())));
        } else {
            logger.log(Logger::ERROR, QString(Q_FUNC_INFO).append(": No game events data found!"));
        }

        // TODO: Instead of callung "eventsUpdated" here, we would simply add the events to the corresponding game.
        emit eventsUpdated(events);
    } else {
        logger.log(Logger::ERROR, QString(Q_FUNC_INFO).append(": Game with ID " + gameId + " not found, skipping update."));
    }
}

// Parse the players
// TODO: Parse the player stats as well
QList<Player *> SIHFDataSource::parsePlayers(QVariantList data) {
    QList<Player *> players;
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Parsing player data."));

    QListIterator<QVariant> iterator(data);
    Player *player;
    while(iterator.hasNext()) {
        QVariantMap tmp = iterator.next().toMap();

        // Get basics
        qulonglong teamId = tmp.value("teamId").toULongLong();
        quint32 playerId = tmp.value("id").toUInt();

        // Get name
        QString name = tmp.value("fullName").toString();
        int index = name.lastIndexOf(" ");
        QString lastName = name.left(index);
        QString firstName = name.right(name.length()-index-1);//.at(index+1);

        // Create the player
        player = new Player(teamId, playerId);
        player->setName(firstName, lastName);
        player->setJerseyNumber(tmp.value("jerseyNumber").toUInt());
        //this->players.insert(playerId, name);

        players.append(player);
    }

    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Found " + QString::number(players.size()) + " players."));
    return players;
}

void SIHFDataSource::parsePlayers(GameData *game, QVariantList data) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Parsing player data."));

    QMap<quint32, Player *> *players = game->getRoster();
    QListIterator<QVariant> iterator(data);
    Player *player;
    while(iterator.hasNext()) {
        QVariantMap tmp = iterator.next().toMap();

        // Get basics
        qulonglong teamId = tmp.value("teamId").toULongLong();
        quint32 playerId = tmp.value("id").toUInt();

        // Get name
        QString name = tmp.value("fullName").toString();
        int index = name.lastIndexOf(" ");
        QString lastName = name.left(index);
        QString firstName = name.right(name.length()-index-1);//.at(index+1);

        // Create the player
        player = new Player(teamId, playerId, game);
        player->setName(firstName, lastName);
        player->setJerseyNumber(tmp.value("jerseyNumber").toUInt());

        // Add the player to the list of players
        players->insert(playerId, player);
#if 0
        int playerIndex = players->indexOf(player);
        if(playerIndex == -1) {
            players->append(player);
        } else {
            players->replace(playerIndex, player);
        }
#endif
    }

    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Found " + QString::number(players->size()) + " players."));
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
    getGameSummaries();
    if(id != NULL) {
        getGameDetails(id);
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
