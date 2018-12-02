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

#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class GameEvent : public QObject {
    Q_OBJECT

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
        int type;

        // Stores the event time in seconds
        float time;

        // List of players involved in this event
        QMap<int, quint32> players;

        // Stores the ID of the team this event belongs to
        qlonglong team;

        // For events of EVENT_TYPE::GOAL only: Stores the score
        QString score;
        QString scoreType;

        // For events of EVENT_TYPE::PENALTY only: Stores the penalty ID and type (minutes)
        int penaltyId;
        QString penaltyType;

        //
        bool penaltyShot;

        // List of human-readable penalty texts
        static QList<QString> penaltyTexts;

    public:
        explicit GameEvent(int type);
        int getType(void);
        void setTime(QString time);
        float getTime(void) const;
        void setTeam(qlonglong team);
        qlonglong getTeam(void);
        void addPlayer(int role, quint32 playerId);
        void setScore(QString score, QString type);
        void setPenalty(int id, QString type);
        int getPenalty(void);
        void setPenaltyShot(bool scored);

        // TODO: These are convenience methods and are to disappear/change in
        // future versions
        quint32 getPlayer(void) const;
        quint32 getPlayer(int role) const;
        QString getAdditionalInfo(void) const;
        QString getEvent(void) const;
        QString getEventSubtext(void) const;

        static bool greaterThan(const GameEvent *e1, const GameEvent *e2);
        static QString getPenaltyText(int id);
};

#endif // GAMEEVENT_H
