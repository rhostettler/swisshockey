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

#include "game.h"
#include "logger.h"

// The list of game status texts
QStringList Game::GameStatusTexts = QStringList()
    << QString("Not Started")    // 0
    << QString("First Period")
    << QString("End of First")
    << QString("Second Period")
    << QString("End of Second")
    << QString("Third Period")   // 5
    << QString("End of Third")
    << QString("Overtime")
    << QString("Shootout")
    << QString("Final (unofficial)")
    << QString("Final (unofficial, overtime)") // 10
    << QString("Final (unofficial, shootout)")
    << QString("Final");

// Initialize the game
Game::Game(QString gameId, QObject *parent) : QAbstractListModel(parent) {
    // Store game ID
    mGameId = gameId;

    // Set the data roles
    // TODO: Consider renaming the roles at some point
    mDataRoleNames[TeamRole] = "eventteam";
    mDataRoleNames[TimeRole] = "eventtime";
    mDataRoleNames[PlayerRole] = "eventplayer";
    mDataRoleNames[AdditionalInfoRole] = "eventinfo";
    mDataRoleNames[EventRole] = "eventtext";
    mDataRoleNames[EventSubtextRole] = "eventsubtext";
}

// Update the game score/summary
void Game::updateSummary(QVariantMap data) {
    // Update the score and trigger a signal if it changed
    QMap<QString, QString> oldScore = mScore;
    QVariantMap newScore = data["score"].toMap();
    mScore["first"] = newScore.value("first", "-:-").toString();
    mScore["second"] = newScore.value("second", "-:-").toString();
    mScore["third"] =  newScore.value("third", "-:-").toString();
    if(newScore.contains("overtime")) {
        mScore["overtime"] =  newScore.value("overtime", "-:-").toString();
    }
    mScore["total"] = newScore["total"].toString();
    if(mScore["total"] != oldScore["total"] && oldScore["total"] != "-:-") {
        emit scoreChanged();
    }

    // Update the game status and trigger a signal if it changed
    int oldStatus = mGameStatus;
    mGameStatus = data["status"].toInt();
    if(this->mGameStatus != oldStatus) {
        emit statusChanged();
    }
}

// TODO: Remove once we have migrated everything.
#if 0
void Game::updateEvents(QList<Event *> events) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Updating game events."));

    // Clear all events
    beginResetModel();
    mEventList.clear();
    endResetModel();

    // Update the list of events (if there are any)
    QListIterator<Event *> iter(events);
    while(iter.hasNext()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        mEventList.append(iter.next());
        endInsertRows();
    }

    // Sort the events
    // TODO: Consider sorting upside down when the game is in progress and increasing when the game is finished
    layoutAboutToBeChanged();
    qSort(mEventList.begin(), mEventList.end(), Event::greaterThan);
    layoutChanged();
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Added " + QString::number(mEventList.size()) + " events."));
}
#endif

#if 0
// Parses the player list and adds them to the local list of players where we
// have a player license <=> player name map
void GameData::updateRosters(QList<Player *> players) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Updating rosters."));

    mRoster.clear();
    QListIterator<Player *> iter(players);
    while(iter.hasNext()) {
        Player *player = iter.next();
        mRoster.insert(player->getPlayerId(), player);
    }

    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Added " + QString::number(mRoster.size()) + " players to roster."));
}
#endif

QString Game::getGameId(void){
    return mGameId;
}

void Game::setLeague(QString leagueId) {
    mLeagueId = leagueId;
}

QString Game::getLeague() {
    return this->mLeagueId;
}

void Game::setDateTime(QString time) {
    mStartTime = time;
}

void Game::setHometeam(QString id, QString name) {
    mHometeamId = id.toLongLong();
    mHometeamName = name;
}

QString Game::getHometeam() {
    return mHometeamName;
}

QString Game::getHometeamId() {
    return QString::number(this->mHometeamId);
}

void Game::setAwayteam(QString id, QString name) {
    mAwayteamId = id.toLongLong();
    mAwayteamName = name;
}

QString Game::getAwayteam() {
    return this->mAwayteamName;
}

QString Game::getAwayteamId() {
    return QString::number(this->mAwayteamId);
}

void Game::setScore(QMap<QString, QString> score) {
    // Update the score and trigger a signal if it changed
    QMap<QString, QString> oldScore = mScore;
    mScore = score;
    if(mScore["total"] != oldScore["total"] && oldScore["total"] != "-:-") {
        emit scoreChanged();
    }
}

QString Game::getTotalScore() {
    return this->mScore["total"];
}

QString Game::getPeriodsScore() {
    QString score = mScore.value("first") + ", " + mScore.value("second") + ", " + mScore.value("third");

    if(mScore.contains("overtime")) {
        score.append(", " + this->mScore.value("overtime"));
    }

    return score;
}

void Game::setStatus(int status) {
    // Update the game status and trigger a signal if it changed
    int oldStatus = mGameStatus;
    mGameStatus = status;
    if(mGameStatus != oldStatus) {
        emit statusChanged();
    }
}

int Game::getGameStatus() {
    return this->mGameStatus;
}

QString Game::getGameStatusText() {
    QString text;
    if(this->mGameStatus == 0) {
        text = "Starts " + this->mStartTime;
    } else {
        text = Game::GameStatusTexts.value(this->mGameStatus, "Unknown status");
    }

    return text;
}

EventList *Game::getEventList(void) {
    return &mEventList;
}

PlayerList *Game::getHometeamRoster() {
    return &mHometeamRoster;
}

PlayerList *Game::getAwayteamRoster() {
    return &mAwayteamRoster;
}

// Implementation of the QAbstractListModel methods
// Returns the number of rows in the list
int Game::rowCount(const QModelIndex &parent) const {
    //return this->mEventList.count();
    return 0;
}

// Returns the data requested by the view
QVariant Game::data(const QModelIndex &index, int role) const {
    QVariant data;
    int key = index.row();
#if 0
    Event *event = this->mEventList[key];

    switch(role) {
            // migration ok
        case TeamRole:
            data = event->getTeam();
            break;

            // migration ok
        case TimeRole: {
                float tmp = event->getTime();
                quint32 minutes = (int)tmp/60;
                quint32 seconds = (int)(tmp-minutes*60);
                QString str;
                str.sprintf("%02u:%02u", minutes, seconds);
                data = str;
            }
            break;

        // migration ok
        case PlayerRole: {
                quint32 id = event->getPlayer();
                Player *player = mRoster.value(id, NULL);
//                Player *player
                if(player != NULL) {
                    data = player->getName();
                } else {
                    data = "";
                }
            }
            break;

        // migration ok
        case AdditionalInfoRole: {
                int type = event->getType();
                switch(type) {
                    case Event::GOAL: {
                            QString text = "";
                            Player *player = mRoster.value(event->getPlayer(Event::FIRST_ASSIST), NULL);
                            if(player != NULL) {
                                text.append(player->getName());
                            }
                            player = mRoster.value(event->getPlayer(Event::SECOND_ASSIST), NULL);
                            if(player != NULL) {
                                text.append(", " + player->getName());
                            }
                            data = text;
                        }
                        break;

                    case Event::PENALTY:
                        data = Event::getPenaltyText(event->getPenalty());
                        break;

                    case Event::GOALKEEPER_IN:
                        data = "Goalkeeper in";
                        break;

                    case Event::GOALKEEPER_OUT:
                        data = "Goalkeeper out";
                        break;

                    case Event::PENALTY_SHOT:
                        data = "Goalkeeper: "; //+ this->players.value(event->getPlayer(GameEvent::GOALKEEPER), "");
                        break;

                    default:
                        break;
                }
            }
            break;

        case EventRole:
            data = event->getEvent();
            break;

        case EventSubtextRole:
            data = event->getEventSubtext();
            break;

        default:
            break;
    }
#endif

    return data;
}

// Returns the header
QVariant Game::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

// Role names for QML
QHash<int, QByteArray> Game::roleNames() const {
    return this->mDataRoleNames;
}
