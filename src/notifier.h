#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QObject>
#include <QModelIndex>
#include <QMap>
#include <QString>
#include <MNotification>
#include <MNotificationGroup>

#include <src/gamedaydata.h>

class Notifier : public QObject {
    Q_OBJECT

    private:
        GamedayData *model;
//        QList<QString> teams
//        MNotificationGroup *notificationGroup;
        QMap<QString, QString> teams;

    public:
        explicit Notifier(GamedayData *model, QWidget *parent = 0);
        bool notificationEnabled(QString team);

    signals:

    public slots:
        void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
};

#endif // NOTIFIER_H
