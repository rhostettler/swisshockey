/*
 * Copyright 2014-present Roland Hostettler
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

#include <QDate>

#include "gamelist.h"
#include "logger.h"

GameList::GameList(QObject *parent) : QAbstractListModel(parent) {
    // Initialize the different data roles that can be used by the ListView
    this->mRoles[HometeamRole] = "hometeam";
    this->mRoles[HometeamIdRole] = "hometeamId";
    this->mRoles[AwayteamRole] = "awayteam";
    this->mRoles[AwayteamIdRole] = "awayteamId";
    this->mRoles[TotalScoreRole] = "totalscore";
    this->mRoles[PeriodsScoreRole] = "periodsscore";
    this->mRoles[GameStatusRole] = "gamestatus";
    this->mRoles[GameIdRole] = "gameid";
    this->mRoles[LeagueRole] = "league";

    // Set the date to <empty>
    // TODO: We should probably work with DateTime instead of strings
    this->mDate = QDate::currentDate().toString("yyyy-MM-dd");;

    // Signal mapper acts as a proxy between the GameData -> GamedayData -> outside world
    this->mSignalMapper = new QSignalMapper(this);
    connect(this->mSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(gamedataChanged(const QString &)));
}

void GameList::gamedataChanged(const QString & key) {
    QModelIndex index = createIndex(this->mGameIndices.indexOf(key.toLongLong()), 0);
    emit dataChanged(index, index);
}

void GameList::addGame(Game *game) {
    // TODO: Add debuggin information

    qulonglong key = game->getGameId().toULongLong();
    if(!mGames.contains(key)) {
        // A game with the given ID is not in the list yet and hence, we add
        // it. For that, we need to call beginInsertRows() and endInsertRows()
        // so that the ListView gets notified about the new content.
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        mGames.insert(key, game);
        mGameIndices.append(key);
        endInsertRows();

        // Listen to the scoreChanged()- and statusChanged()-signals to know
        // when we need to notify the view through the dataChanged()-signal.
        // We have to do this through the SignalMapper because GameData's
        // signals don't take arguments but we need to be able to identify
        // the sender in GamedayData (and the views).
        connect(mGames[key], SIGNAL(scoreChanged()), mSignalMapper, SLOT(map()));
        connect(mGames[key], SIGNAL(statusChanged()), mSignalMapper, SLOT(map()));
        mSignalMapper->setMapping(mGames[key], QString::number(key));
    } else {
        // NOP
    }
}

Game* GameList::getGame(QString gameId) {
    qulonglong key = gameId.toULongLong();
    Game *game;
    if(this->mGames.contains(key)) {
        game = mGames[key];
    } else {
        game = NULL;
    }

    return game;
}

// Impelementation of QAbstractListModel follows below
// Returns the number of rows in the list
int GameList::rowCount(const QModelIndex &parent) const {
    return this->mGames.count();
}

// Returns the data requested by the view
QVariant GameList::data(const QModelIndex &index, int role) const {
    QVariant data;

    // Map from the ListView index to the game key
    qulonglong key = this->mGameIndices[index.row()];

    switch(role) {
        case HometeamRole:
            data = this->mGames[key]->getHometeam();
            break;

        case HometeamIdRole:
            data = this->mGames[key]->getHometeamId();
            break;

        case AwayteamRole:
            data = this->mGames[key]->getAwayteam();
            break;

        case AwayteamIdRole:
            data = this->mGames[key]->getAwayteamId();
            break;

        case TotalScoreRole:
            data = this->mGames[key]->getTotalScore();
            break;

        case PeriodsScoreRole:
            data = this->mGames[key]->getPeriodsScore();
            break;

        case GameStatusRole:
            data = this->mGames[key]->getStatusString();
            break;

        case GameIdRole:
            data = QString::number(key);
            break;

        case LeagueRole:
        data = this->mGames[key]->getLeague();
            break;

        default:
            break;
    }

    return data;
}

// Role names
QHash<int, QByteArray> GameList::roleNames() const {
    return this->mRoles;
}
