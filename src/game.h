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


/*
 * TODO:
 *  * This should not be a AbstractListModel anymore; these things are delegated to EventList and PlayerList, respectively.
 */

#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QVariantMap>
#include <QMap>
#include <QAbstractListModel>

#include "eventlist.h"
#include "event.h"

#include "playerlist.h"
#include "player.h"

class Game : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(QString hometeamId READ getHometeamId CONSTANT)
    Q_PROPERTY(QString hometeamName READ getHometeam CONSTANT)
    Q_PROPERTY(QString awayteamId READ getAwayteamId CONSTANT)
    Q_PROPERTY(QString awayteamName READ getAwayteam CONSTANT)
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

        // List of events and rosters
        EventList mEventList;
        PlayerList mHometeamRoster;
        PlayerList mAwayteamRoster;

        // Status
        // TODO: There should be a (public) enum with statuses, which are rendered in the UI to plain text
        int mGameStatus;

        //
        static QStringList GameStatusTexts;

    public:
        explicit Game(QString gameId, QObject *parent = 0);
        void updateSummary(QVariantMap data);
        void updateGoals(QVariantList goals);
        void updateFouls(QVariantList fouls);
        bool hasChanged(void);
        bool hasChanged(QString type);

        // Getters and setters
        QString getGameId();

        void setLeague(QString leagueId);
        QString getLeague();

        void setDateTime(QString time);

        void setHometeam(QString id, QString name);
        QString getHometeam();
        QString getHometeamId();

        void setAwayteam(QString id, QString name);
        QString getAwayteam();
        QString getAwayteamId();

        void setScore(QMap<QString, QString> score);
        QString getTotalScore();
        QString getPeriodsScore();

        void setStatus(int status);
        int getGameStatus(); // TODO: Unify notation; remove the "Game" in the getters
        QString getGameStatusText();

        EventList *getEventList(void);


        PlayerList *getHometeamRoster(void);
        PlayerList *getAwayteamRoster(void);

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
        QHash<int, QByteArray> roleNames() const;

        static QString parsePlayerName(QString name);

    signals:
        void scoreChanged(void);
        void statusChanged(void);

    public slots:
#if 0
        //void updateEvents(QList<Event *> gameEvents);
        //void updateRosters(QList<Player *> players);
#endif
};

#endif // GAMEDATA_H