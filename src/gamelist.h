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

#ifndef GAMELIST_H
#define GAMELIST_H

#include <QAbstractListModel>
#include <QSignalMapper>

#include "game.h"

class GameList : public QAbstractListModel {
    Q_OBJECT

    private:
        QHash<int, QByteArray> roles;
        QMap<qulonglong, Game *> mGames;
        QList<qulonglong> mGameIndices;
        QString mDate; // TODO: This should be part of the Games, then we wouldn't have to bother about keeping different games. Could also add filters for the games later on.
        QSignalMapper *mSignalMapper;

    public:
        explicit GameList(QObject *parent = 0);

        // date: date of the gameday, data: the list of games
        void updateGames(QString date, QVariantMap data);

        void addGame(Game *game);
        Game * getGame(QString gameId);

        // implementations of interface QAbstractListModel
        enum GameRoles {
            HometeamRole = Qt::UserRole + 1,
            HometeamIdRole,
            AwayteamRole,
            AwayteamIdRole,
            TotalScoreRole,
            PeriodsScoreRole,
            GameStatusRole,
            GameIdRole,
            LeagueRole
        };

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        QHash<int, QByteArray> roleNames() const;

    signals:

    public slots:
//        void updateData(QVariantMap data);
        void gamedataChanged(const QString & key);
};

#endif // GAMEDAYDATA_H
