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

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject {
    Q_OBJECT

    private:
        qulonglong mTeamId;
        quint32 mId;
        QString mFirstName;
        QString mLastName;

    public:
        explicit Player(quint32 id, QString firstName, QString lastName, qulonglong teamId, QObject *parent = 0);

    signals:

    public slots:
};

#endif // PLAYER_H
