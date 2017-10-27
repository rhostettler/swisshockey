/*
 * Copyright 2017 Roland Hostettler
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

#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QVariantMap>
#include <QMap>
#include <QAbstractListModel>

#include "gamedata.h"
#include "gameevent.h"


class GameData : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(QString hometeamId READ getHometeamId)
    Q_PROPERTY(QString awayteamId READ getAwayteamId)
    Q_PROPERTY(QString totalScore READ getTotalScore NOTIFY scoreChanged)
    Q_PROPERTY(QString periodsScore READ getPeriodsScore NOTIFY scoreChanged)
    Q_PROPERTY(int gameStatus READ getGameStatus NOTIFY statusChanged)

    private:
        QString mGameId;
        QString mLeagueId;

        QHash<int, QByteArray> mDataRoleNames;

        // Home- and away team names & IDs
        QString mHometeamName;
        qulonglong mHometeamId;
        QString mAwayteamName;
        qulonglong mAwayteamId;

        // Time
        QString mStartTime;

        // Score
        QMap<QString, QString> mScore;

        // List of events
        QList<GameEvent *> mGameEvents;

        // List of players
        QMap<quint32, QString> players;

        // Status
        int mGameStatus;

        //
        static QStringList mGameStatusTexts;

    public:
        explicit GameData(QVariantMap data, QObject *parent = 0);
        void updateSummary(QVariantMap data);
        void updateGoals(QVariantList goals);
        void updateFouls(QVariantList fouls);
        bool hasChanged(void);
        bool hasChanged(QString type);

        QString getLeague();
        QString getHometeam();
        QString getHometeamId();
        QString getAwayteam();
        QString getAwayteamId();
        QString getTotalScore();
        QString getPeriodsScore();
        int getGameStatus();
        QString getGameStatusText();

        // implementations of interface QAbstractListModel
        enum EventRoles {
            TeamRole = Qt::UserRole + 1,
            TimeRole,
            PlayerRole,
            AdditionalInfoRole,
            EventRole,
            EventSubtextRole
        };

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
#ifdef PLATFORM_SFOS
        QHash<int, QByteArray> roleNames() const;
#endif

        static QString parsePlayerName(QString name);

    signals:
        void scoreChanged(void);
        void statusChanged(void);

    public slots:
        void updateEvents(QList<GameEvent *> mGameEvents);
        void updatePlayers(QVariantList players);
};

#endif // GAMEDATA_H
