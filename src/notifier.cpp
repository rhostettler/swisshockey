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

#include "notifier.h"
#include "config.h"
#include "gamedata.h"
#include "logger.h"

Notifier::Notifier(GamedayData *games, QObject *parent) : QObject(parent) {
    mGames = games;
    mGame = NULL;
#if 0
    connect(mGames, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(dataChanged(QModelIndex, QModelIndex)));
#endif
    //mEnabled = false;
    mEnabled = true;
}

// Update the game to send notifications for
void Notifier::setGameId(QString id) {
    if(mGame != NULL) {
        bool status = disconnect(mGame, SIGNAL(scoreChanged()), this, SLOT(scoreChanged()));
    }
    if(id != NULL) {
        mGame = mGames->getGame(id);
        // TODO: What if getGame() doesn't return a game?
        bool status = connect(mGame, SIGNAL(scoreChanged()), this, SLOT(scoreChanged()));
    }
}

// Enable notifications
void Notifier::enableNotifications(void) {
    this->mEnabled = true;
}

// Disable notifications
void Notifier::disableNotifications(void) {
    this->mEnabled = false;
}

// Remove all notifications
void Notifier::clearNotifications(void) {
    Notification *notification;
    QList<QObject *> notifications = Notification::notifications();
    QListIterator<QObject *> iter(notifications);
    while(iter.hasNext()) {
        notification = (Notification *)iter.next();
        notification->close();
        delete(notification);
    }
}

void Notifier::sendNotification(GameData *game) {
    if(mEnabled && mGame != NULL) {
        // Notification body
        // TODO: Should use team abbreviations in summary, otherwise the text is too long
        QString summary(mGame->getHometeam() + " - " + mGame->getAwayteam() + " " + mGame->getTotalScore());

        // TODO: Implement preview. Not yet possible since scoreChanged() is triggered by the parseSummary()
        // rather than parseEvents(). Requires some rethinking.
#if 0
        QString preview("XX:YY N. N. (N. N., N. N.)");
#endif

        // Try to find an existing notification for the game first in order to
        // update that one
        bool found = false;
        Notification *notification;
        QList<QObject *> notifications = Notification::notifications();
        QListIterator<QObject *> iter(notifications);
        while(iter.hasNext() && !found) {
            notification = (Notification *)iter.next();
            if(mGame->getGameId().toULong() == notification->replacesId()) {
                // TODO: It appears like this branch is broken, i.e. the 'RelacesId'-part doesn't work yet
                found = true;
                notification->setPreviewSummary(summary);
                notification->setSummary(summary);
#if 0
                notification->setPreviewBody(preview);
#endif
                notification->setHintValue("x-nemo-feedback", "chat,chat_exists"); // <- only sounds when not in lock screen
                notification->setHintValue("x-nemo-priority", 100);
                notification->setHintValue("x-nemo-display-on", true);
                notification->setHintValue("x-nemo-led-disabled-without-body-and-summary", false);
                notification->setItemCount(notification->itemCount()+1);
            }
        }

        // If no existing notification was found, we'll create a new one
        if(!found) {
            notification = new Notification(this);
            notification->setAppName(APP_NAME);
            notification->setPreviewSummary(summary);
            notification->setSummary(summary);
#if 0
            notification->setPreviewBody(preview);
#endif
            notification->setHintValue("x-nemo-feedback", "chat,chat_exists");
            notification->setHintValue("x-nemo-priority", 100);
            notification->setHintValue("x-nemo-display-on", true);
            notification->setHintValue("x-nemo-led-disabled-without-body-and-summary", false);
            notification->setReplacesId(mGame->getGameId().toULong());
        }

        // TODO: Add the callback action

        // Publish / update the notification
        notification->publish();
    }
}

// Notification when the score changed
void Notifier::scoreChanged(void) {
    this->sendNotification(mGame);
}

// TODO: This should be removed eventually
void Notifier::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight) {
#if 0
    // Get the necessary information for the notification and assemble the notification string
    QString gameId = mGames->data(topLeft, GamedayData::GameIdRole).toString();
    GameData *game = mGames->getGame(gameId);
    this->sendNotification(game);
#endif
}

// Check if the notification for a given team is enabled
bool Notifier::notificationEnabled(QString team) {
    // TODO: Re-implement this, requires config stuff (actually, remove once we have the new GamedayData structure in place)
#if 0
    QString key = teams[team];
    Config& config = Config::getInstance();
    return config.getValue(key, false).toBool();
#endif
    return true;
}

// Destructor
Notifier::~Notifier(void) {
    // Clear all notifications when exiting
    this->clearNotifications();
}
