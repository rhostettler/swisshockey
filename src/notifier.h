/*
 * Copyright 2014-2018 Roland Hostettler
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

#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QModelIndex>
#include <QMap>
#include <QString>
#include <nemonotifications-qt5/notification.h>

#include "gamedata.h"
#include "gamedaydata.h"

class Notifier : public QObject {
    Q_OBJECT

    private:
        bool mEnabled;
        GamedayData *mGames;
        GameData *mGame;

    public:
        explicit Notifier(GamedayData *games, QObject *parent = 0);
        ~Notifier(void);
        void setGameId(QString id);
        void sendNotification(GameData *game);
        void enableNotifications(void);
        void disableNotifications(void);
        void clearNotifications(void);
        bool notificationEnabled(QString team);

    signals:

    public slots:
        void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
        void scoreChanged(void);
};

#endif // NOTIFIER_H
