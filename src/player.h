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

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject {
    Q_OBJECT

    Q_PROPERTY(quint8 jerseyNumber READ getJerseyNumber)
    Q_PROPERTY(quint8 lineNumber READ getLineNumber)
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(quint8 position READ getPosition)

    private:
        const qulonglong mTeamId;
        const quint32 mPlayerId;
        QString mFirstName;
        QString mLastName;
        quint8 mJerseyNumber;
        quint8 mLineNumber;
        quint8 mPosition;
        //QVector<QString> mStats;

    public:
        enum PLAYER_POSITION {
            POSITION_GK,
            POSITION_LD,
            POSITION_RD,
            POSITION_LW,
            POSITION_RW,
            POSITION_C
        };

        enum PLAYER_STATS {
            STATS_GOALS = 0,
            STATS_ASSISTS,
            STATS_POINTS,
            STATS_PIM,
            STATS_SOG,
            STATS_FO,
            STATS_TOI,
        };

        enum GK_STATS {
            STATS_GK_GA = 0,
            STATS_GK_SVS,
            STATS_GK_SVSP,
            STATS_GK_PIM,
            STATS_GK_TOI
        };

    public:
        explicit Player(qulonglong teamId, quint32 playerId, QObject *parent = 0);

        quint32 getPlayerId() const;

        void setName(QString firstName, QString lastName);
        QString getName() const;

        void setJerseyNumber(quint8 jerseyNumber);
        quint8 getJerseyNumber() const;

        void setLineNumber(quint8 lineNumber);
        quint8 getLineNumber(void) const;

        void setPosition(quint8 position);
        quint8 getPosition(void) const;

        static bool greaterThan(const Player *p1, const Player *p2);
        static bool lessThan(const Player *p1, const Player *p2);

        // Comparison operators
        // TODO: Fix these. They are needed such that we can use QVector.indexOf(playerId) rather
        // than using a map for accessing players via their playerId.
        // However, that does not solve the problem of finding players by their jersey number
        bool operator ==(Player const &other) const;
        bool operator !=(Player const &other) const;
};

#endif // PLAYER_H
