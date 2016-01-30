#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <QObject>
#include <QString>
#include <QList>

class GameEvent : public QObject {
    Q_OBJECT

    private:
        QString time;
        QString player;
        QString additionalInfo;
        QString event;

        static QList<QString> penaltyTexts;

    public:
        explicit GameEvent(QString time, QString player, QString additionalInfo,
                           QString event, QObject *parent = 0);
        QString getTime(void) const;
        QString getPlayer(void) const;
        QString getAdditionalInfo(void) const;
        QString getEvent(void) const;
        QString getEventSubtext(void) const;

        static bool greaterThan(const GameEvent *e1, const GameEvent *e2);
        static QString getPenaltyText(int id);

    signals:

    public slots:
    
};

#endif // GAMEEVENT_H
