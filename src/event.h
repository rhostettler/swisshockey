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

#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

/* TODO:
 * * This class is quite a mess still, there's plenty of confusion and no separation of concerns between gamedata, gameevent and the details page
 */

#include "player.h"

class Event : public QObject {
    Q_OBJECT

    Q_PROPERTY(qlonglong mTeam READ getTeam CONSTANT)
    Q_PROPERTY(QString time READ getTimeString CONSTANT)
    Q_PROPERTY(QString player READ getPlayerString CONSTANT) // TODO: Should be migrated to getPlayer
    Q_PROPERTY(QString info READ getInfo CONSTANT)
    Q_PROPERTY(QString value READ getValue CONSTANT)
    Q_PROPERTY(QString context READ getContext CONSTANT)

    public:
        enum EventType {
            GOAL = 1,
            PENALTY,
            PENALTY_SHOT,
            GOALKEEPER_OUT,
            GOALKEEPER_IN
        };

        // TODO: I should find better names for these I think.
        enum PlayerRole {
            SCORER = 1,
            FIRST_ASSIST,
            SECOND_ASSIST,
            PENALIZED,
            GOALKEEPER
        };

    private:
        // Stores the event type
        int mType;

        // Stores the event time in seconds
        float mTime;

        // List of players involved in this event
        QMap<int, Player *> mPlayers;

        // Stores the ID of the team this event belongs to
        qlonglong mTeam;

        // For events of EVENT_TYPE::GOAL only: Stores the score
        QString mScore;
        QString mScoreType;

        // For events of EVENT_TYPE::PENALTY only: Stores the penalty ID and type (minutes)
        int mPenaltyId;
        QString mPenaltyType;

        // TODO: Can this be handled through mPenaltyType or one of the other above?
        bool mPenaltyShot;

        // List of human-readable penalty texts
        static QList<QString> penaltyTexts;

    public:
        explicit Event(int mType);
        int getType(void);

        void setTime(QString mTime);
        float getTime(void) const;
        QString getTimeString(void) const;

        void setTeam(qlonglong mTeam);
        qlonglong getTeam(void);

        void addPlayer(int role, Player *player);
        void setScore(QString mScore, QString mType);
        void setPenalty(int id, QString mType);
        int getPenalty(void);
        void setPenaltyShot(bool scored);


        // TODO: Need to code these
        // Property getters/setters
        //QString getEventTime(void) const;
        //QString getPlayer(void) const;  // TODO: There's already a getPlayer further down; how do they relate? I think the problem is that we only store a license number in the gameevent but we want to return a string to the view. not sure what the best solution is to this.


        // TODO: These are convenience methods and are to disappear/change in
        // future versions
        Player *getPlayer(void) const;
        Player *getPlayer(int role) const;
        QString getPlayerString(void) const;

#if 0
        QString getAdditionalInfo(void) const;
#endif
        QString getValue(void) const;
        QString getInfo(void) const;
        QString getContext(void) const;

        static bool greaterThan(const Event *e1, const Event *e2);
        static bool lessThan(const Event *e1, const Event *e2);
        static QString getPenaltyText(int id);
};

#endif // GAMEEVENT_H
