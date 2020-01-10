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

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject {
    Q_OBJECT

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
            POSITION_GOALKEEPER,
            POSITION_DEFENCE,
            POSITION_LEFT_WING,
            POSITION_RIGHT_WING,
            POSITION_CENTER
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

        // Comparison operators
        // TODO: Fix these. They are needed such that we can use QVector.indexOf(playerId) rather
        // than using a map for accessing players via their playerId.
        // However, that does not solve the problem of finding players by their jersey number
        bool operator ==(Player const &other) const;
        bool operator !=(Player const &other) const;

    signals:

    public slots:
};

#endif // PLAYER_H
