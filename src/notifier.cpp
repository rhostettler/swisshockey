#include "notifier.h"
#include <MNotification>
#include <QTextStream>
#include <gconfitem.h>

Notifier::Notifier(GamedayData *model, QWidget *parent) : QObject(parent) {
    // Add all the teams to the teams -> GConf key map
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

    // Initialize the Notification group
/*    this->notificationGroup = new MNotificationGroup("x-nokia.messaging.im",
        "NL Live Scores", "");
    this->notificationGroup->setImage("file:///usr/share/icons/hicolor/80x80/apps/NLLiveScores80.png");
    this->notificationGroup->publish();*/
}

void Notifier::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight) {
    // TODO: We'll just get the whole game and then extract the necessary information from there! (we manage the notifications ourselves afterwards and keep just one notification per game, max)

    // Get the necessary information for the notification and assemble the notification string
    QString hometeam = this->model->data(topLeft, GamedayData::HometeamRole).toString();
    QString awayteam = this->model->data(topLeft, GamedayData::AwayteamRole).toString();
    QString score = this->model->data(topLeft, GamedayData::TotalScoreRole).toString();

    // TODO: Here, we shoul d implement a filter so that we only notify when a team of interest changes
    if(this->notificationEnabled(hometeam) || this->notificationEnabled(awayteam)) {
        QString body = hometeam.append(" - ").append(awayteam).append(" ").append(score);

        // Send a notification
        MNotification notification("livescores.score.update", "Live Scores", body);
        //notification.setGroup(*(this->notificationGroup));
        notification.publish();

        // Update the notification group
    /*  this->notificationGroup->setCount(this->notificationGroup->count()+1);
        this->notificationGroup->setBody(QString::number(this->notificationGroup->count()).append(" updates."));
        this->notificationGroup->publish();*/
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
