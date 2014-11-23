#include "notifier.h"
#include <MNotification>
#include <gconfitem.h>

Notifier::Notifier(GamedayData *model, QWidget *parent) : QObject(parent) {
    // Add all the teams to the teams->GConf key map
    teams.insert("Ambri-Piotta", "hcap");
    teams.insert("SC Bern", "scb");
    teams.insert("EHC Biel", "ehcb");
    teams.insert("HC Davos", "hcd");
    teams.insert("Fribourg", "hcfg");
    teams.insert("Geneve", "hcgs");
    teams.insert("Kloten Flyers", "ehck");
    teams.insert("Lakers", "scrj");
    teams.insert("Lausanne HC", "lhc");
    teams.insert("ZSC Lions", "zsc");
    teams.insert("HC Lugano", "hcl");
    teams.insert("EV Zug", "evz");

    // Set the model and connect the observer
    this->model = model;
    connect(this->model, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(dataChanged(QModelIndex, QModelIndex)));
}

void Notifier::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight) {
    // BUG: As of now, this will also fire a notification when the game status is updated. I need to fix that somehow.

    // Get the necessary information for the notification and assemble the notification string
    QString hometeam = this->model->data(topLeft, GamedayData::HometeamRole).toString();
    QString awayteam = this->model->data(topLeft, GamedayData::AwayteamRole).toString();
    QString score = this->model->data(topLeft, GamedayData::TotalScoreRole).toString();
    QString gameId = this->model->data(topLeft, GamedayData::GameIdRole).toString();

    // Check if notifications for the changed team are enabled and create/update
    // a notification if so
    if(this->notificationEnabled(hometeam) || this->notificationEnabled(awayteam)) {
        // Notification body
        QString body = hometeam.append(" - ").append(awayteam).append(" ").append(score);

        // Try to find an existing notification for the game first in order to
        // update that one
        bool found = false;
        MNotification *notification;
        QList<MNotification *> notifications = MNotification::notifications();
        QListIterator<MNotification *> iter(notifications);
        while(iter.hasNext() && !found) {
            notification = iter.next();
            if(gameId.compare(notification->identifier()) == 0) {
                found = true;
                notification->setBody(body);
            }
        }

        // If no existing notification was found, we'll create a new one
        if(!found) {
            notification = new MNotification("livescores.score.update", "Live Scores", body);
            notification->setIdentifier(gameId);
        }

        // Publish / update the notification
        notification->publish();
    }
}

bool Notifier::notificationEnabled(QString team) {
    QString key = teams[team];
    QString gconfPath = "/apps/NLLiveScores/settings/";
    QString gconfKey = gconfPath.append(key);

    GConfItem item(gconfKey);
    bool status = item.value(false).toBool();

    return status;
}
