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

// Compares two events e1 and e2
bool GameEvent::greaterThan(const GameEvent *e1, const GameEvent *e2) {
    bool less = false;

    // Calculate the time difference between the two events
    QTime time1 = QTime::fromString(e1->getTime(), "m:ss");
    QTime time2 = QTime::fromString(e2->getTime(), "m:ss");
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
    << QString("UIllegal equipment")
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
