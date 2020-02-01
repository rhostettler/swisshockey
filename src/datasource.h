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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QVariantMap>

#include "gamelist.h"
#include "event.h"
#include "player.h"

class DataSource : public QObject {
    Q_OBJECT

    protected:
        GameList *mGamesList;

    public:
        explicit DataSource(GameList *gamesList, QObject *parent = 0);

        virtual void update(QString id) = 0;

    signals:
        void summaryUpdated(QVariantMap data);
        void eventsUpdated(QList<Event *> events);
        void playersUpdated(QList<Player *> players);
        void updateError(QString message);
        void updateStarted();
        void updateFinished();

    public slots:
    
};

#endif // DATASOURCE_H
