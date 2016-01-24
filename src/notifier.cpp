#include "notifier.h"
#include <MNotification>
//#include <gconfitem.h>

#include "config.h"

// TODO: Instead of havin a map from team-name to team abbreviation and looking
// things up that way, I should preferrably use the team id as gconf keys and
// check if the notifications are enabled in that way. This will make the
// 'teams'-list unnecessary and everything much more generic.
// TODO: In accordance with the above, I might want to write my own CP applet
// that allows for better configuration as well as a Config signleton that can
// be used throughout the app for easy access to the config values.
Notifier::Notifier(GamedayData *model, QWidget *parent) : QObject(parent) {
    // TODO: Connect to dbus and create a dbus interface like so:
    /* DEFAULT_NAME = 'com.tgalal.meego.MNotificationManager'
    DEFAULT_PATH = '/MNotificationManager'
    DEFAULT_INTF = 'com.tgalal.meego.MNotificationManager'

    def __init__(self,source_name, source_display_name, on_data_received=None):
        dbus_main_loop = dbus.glib.DBusGMainLoop(set_as_default=True)
        session_bus = dbus.SessionBus(dbus_main_loop)
        self.userId = 0#os.geteuid();

        self.local_name = '.'.join([self.DEFAULT_NAME, source_name])
        print  self.local_name
        bus_name = dbus.service.BusName(self.local_name, bus=session_bus)

        dbus.service.Object.__init__(self,object_path=self.DEFAULT_PATH,bus_name=bus_name)



        self.next_action_id = 1
        self.actions = {}
        self.source_name = source_name
        self.source_display_name = source_display_name
        self.on_data_received = on_data_received

        o = session_bus.get_object(self.INTERFACE, self.PATH)
        self.proxy = dbus.Interface(o, self.INTERFACE)

        self.userId = self.proxy.notificationUserId()


    And then we can use something like:
@dbus.service.method(DEFAULT_INTF)
    def ReceiveActionCallback(self, action_id):

        action_id = int(action_id)
        callable = self.actions[action_id]
        callable()
*/


    // Add all the teams to the teams->GConf key map
    teams.insert("HC Ambri-Piotta", "hcap");
    teams.insert("SC Bern", "scb");
    teams.insert("EHC Biel", "ehcb");
    teams.insert("HC Davos", "hcd");
    teams.insert("Fribourg-Gottéron", "hcfg");
    teams.insert("Genève-Servette HC", "hcgs");
    teams.insert("Kloten Flyers", "ehck"); // TODO: Name in data needs to be verified
//    teams.insert("Lakers", "scrj");
    teams.insert("Lausanne HC", "lhc");
    teams.insert("ZSC Lions", "zsc");      // TODO: Name in data needs to be verified
    teams.insert("HC Lugano", "hcl");
    teams.insert("SCL Tigers", "scl");
    teams.insert("EV Zug", "evz");

    // Disable the notifier by default (assuming that we go to FG directly upon
    // start)
    this->enabled = false;

    // Set the model and connect the observer
    this->model = model;
    connect(this->model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(dataChanged(QModelIndex, QModelIndex)));
}

void Notifier::enableNotifications(void) {
    this->enabled = true;
}

void Notifier::disableNotifications(void) {
    this->enabled = false;
}

void Notifier::clearNotifications(void) {
    // Remove all the notificationss
    MNotification *notification;
    QList<MNotification *> notifications = MNotification::notifications();
    QListIterator<MNotification *> iter(notifications);
    while(iter.hasNext()) {
        notification = iter.next();
        notification->remove();
        delete(notification);
    }
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
    if((this->notificationEnabled(hometeam) || this->notificationEnabled(awayteam)) && this->enabled) {
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
                notification->setCount(notification->count()+1);
            }
        }

        // If no existing notification was found, we'll create a new one
        if(!found) {
            notification = new MNotification("livescores.score.update", "Live Scores", body);
            notification->setIdentifier(gameId);
        }

        // Add the callback action

        // Publish / update the notification
        notification->publish();
    }
}

// Check if the notification for a given team is enabled
bool Notifier::notificationEnabled(QString team) {
    QString key = teams[team];
    Config& config = Config::getInstance();
    return config.getValue(key, false).toBool();
}
