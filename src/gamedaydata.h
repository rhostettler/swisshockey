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

#ifndef GAMEDAYDATA_H
#define GAMEDAYDATA_H

#include <QAbstractListModel>
#include <QSignalMapper>

#include "gamedata.h"

class GamedayData : public QAbstractListModel {
    Q_OBJECT

    private:
        QHash<int, QByteArray> roles;
        QMap<qulonglong, GameData *> games;
        QList<qulonglong> gameIndices;
        QString date;
        QSignalMapper *signalMapper;

    public:
        explicit GamedayData(QObject *parent = 0);

        // date: date of the gameday, data: the list of games
        void updateGames(QString date, QVariantMap data);

        GameData * getGame(QString id);

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
#ifdef PLATFORM_SFOS
        QHash<int, QByteArray> roleNames() const;
#endif

    signals:

    public slots:
        void updateData(QVariantMap data);
        void gamedataChanged(const QString & key);
    
};

#endif // GAMEDAYDATA_H
