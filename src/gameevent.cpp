#include "gameevent.h"

#include <QTime>
#include <QTextStream>

GameEvent::GameEvent(QString time, QString player, QString additionalInfo,
                     QString event, QObject *parent) : QObject(parent) {

    this->time = time;
    this->player = player;
    this->additionalInfo = additionalInfo;
    this->event = event;
}

QString GameEvent::getTime(void) const {
    return this->time;
}

QString GameEvent::getPlayer(void) const {
    return this->player;
}

QString GameEvent::getAdditionalInfo(void) const {
    return this->additionalInfo;
}

QString GameEvent::getEvent(void) const {
    return this->event;
}

QString GameEvent::getEventSubtext(void) const {
    return "";
}

// Compares two events e1 and e2
bool GameEvent::greaterThan(const GameEvent *e1, const GameEvent *e2) {
    bool less = false;

    QString t1 = e1->getTime();
    if(!QString::compare(t1.at(0), "6")) {
        t1.replace(0, 1, "0");
        t1.prepend("1:");
    } else {
        t1.prepend("0:");
    }

    QString t2 = e2->getTime();
    if(!QString::compare(t2.at(0), "6")) {
        t2.replace(0, 1, "0");
        t2.prepend("1:");
    } else {
        t2.prepend("0:");
    }

    // Calculate the time difference between the two events
    QTime time1 = QTime::fromString(t1, "h:mm:ss");
    QTime time2 = QTime::fromString(t2, "h:mm:ss");
    int deltaTime = time1.secsTo(time2);

    // Positive delta => time2-time1 > 0 => time1 < time2
    if(deltaTime >= 0) {
        less = true;
    } else {
        less = false;
    }

    return !less;
}

QList<QString> GameEvent::penaltyTexts = QList<QString>()
    << QString("Other penalty")
    << QString("Abuse of officials by team officials")
    << QString("Women body check")
    << QString("Abuse of officials by player")
    << QString("Boarding")
    << QString("Butt-Ending")
    << QString("Charging")
    << QString("Checking to the head")
    << QString("Checking from behind")
    << QString("Cross-checking")
    << QString("Delaying the game")
    << QString("Moving the goal")
    << QString("Elbowing")
    << QString("Fisticuffs")
    << QString("Game misconduct penalty")
    << QString("Highsticking")
    << QString("Holding an opponent")
    << QString("Hooking")
    << QString("Holding the stick")
    << QString("Illegal equipment")
    << QString("Interference")
    << QString("Kneeing")
    << QString("Match penalty")
    << QString("Misconduct penalty")
    << QString("Other penalty")
    << QString("Penalty shot")
    << QString("Roughing")
    << QString("Slashing")
    << QString("Spearing")
    << QString("Too many players on ice")
    << QString("Tripping")
    << QString("Abuse of officials")
    << QString("Clipping")
    << QString("Throwing stick or object")
    << QString("Illegal change of players");

QString GameEvent::getPenaltyText(int id) {
    return GameEvent::penaltyTexts.value(id, "Unknown penalty");
}
