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

#include "event.h"
#include "logger.h"

Event::Event(int type) {
    this->mType = type;
}

int Event::getType(void) {
    return this->mType;
}

void Event::setTime(QString time) {
    QStringList split = time.split(":");
    if(split.size() == 2) {
        this->mTime = 60*split[0].toFloat() + split[1].toFloat();
    } else {
        this->mTime = 0;
    }
}

float Event::getTime(void) const {
    return mTime;
}

QString Event::getTimeString(void) const {
    quint32 minutes = (int) mTime/60;
    quint32 seconds = (int)(mTime-minutes*60);
    QString time;
    time.sprintf("%02u:%02u", minutes, seconds);
    return time;
}

void Event::setTeam(qlonglong team) {
    this->mTeam = team;
}

qlonglong Event::getTeam(void) {
    return this->mTeam;
}

void Event::addPlayer(int role, Player *player){
    this->mPlayers[role] = player;
}

void Event::setScore(QString score, QString type) {
    if(this->mType == Event::GOAL) {
        this->mScore = score;
        this->mScoreType = type;
    }
}

void Event::setPenalty(int id, QString type) {
    if(this->mType == Event::PENALTY) {
        this->mPenaltyId = id;
        this->mPenaltyType = type;
    }
}

int Event::getPenalty(void) {
    return this->mPenaltyId;
}

void Event::setPenaltyShot(bool scored) {
    this->mPenaltyShot = scored;
}

Player *Event::getPlayer(void) const {
    int role = -1;
    switch(this->mType) {
        case Event::GOAL:
            role = Event::SCORER;
            break;

        case Event::PENALTY:
            role = Event::PENALIZED;
            break;

        case Event::GOALKEEPER_IN:
            role = Event::GOALKEEPER;
            break;

        case Event::GOALKEEPER_OUT:
            role = Event::GOALKEEPER;
            break;

        case Event::PENALTY_SHOT:
            role = Event::SCORER;
            break;

        default:
            role = -1;
            break;
    }
    return this->mPlayers[role];
}

Player *Event::getPlayer(int role) const {
    return this->mPlayers[role];
}

QString Event::getPlayerString(void) const {
    Player *player = getPlayer();
    return player->getName();
}

#if 0
// TODO: These things should be done in the GameData class, really. Kept for
// convenience for now.
QString Event::getAdditionalInfo(void) const {
    QString info("Temporary regression.");
    switch(this->mType) {
        case Event::GOAL:
            break;
        case Event::PENALTY:
            break;
        default:
            break;
    }

    return info;
}
#endif

QString Event::getValue(void) const {
    QString event("");
    switch(this->mType) {
        case Event::GOAL:
            event = mScore;
            break;
        case Event::PENALTY:
            event = mPenaltyType;
            break;
        case Event::PENALTY_SHOT:
            if(mPenaltyShot) {
                event = "X";
            } else {
                event = "O";
            }
            break;
        default:
            break;
    }

    return event;
}

QString Event::getInfo(void) const {
    int type = mType;
    QString text;

    switch(type) {
        case Event::GOAL: {
                Player *player = mPlayers.value(Event::FIRST_ASSIST, nullptr);
                if(player != nullptr) {
                    text.append(player->getName());
                }
                player = mPlayers.value(Event::SECOND_ASSIST, nullptr);
                if(player != nullptr) {
                    text.append(", " + player->getName());
                }
            }
            break;

        case Event::PENALTY:
            text = Event::getPenaltyText(mPenaltyId);
            break;

        case Event::GOALKEEPER_IN:
            text = "Goalkeeper in";
            break;

        case Event::GOALKEEPER_OUT:
            text = "Goalkeeper out";
            break;

        case Event::PENALTY_SHOT:
            text = "Goalkeeper: "; //+ this->players.value(event->getPlayer(GameEvent::GOALKEEPER), "");
            break;

        default:
            break;
    }

    return text;
}

QString Event::getContext(void) const {
    QString info;
    if(this->mType == Event::GOAL) {
        info = this->mScoreType;
    }

    return info;
}

// Compares if e1 > e2
bool Event::greaterThan(const Event *e1, const Event *e2) {
    float t1 = e1->getTime();
    float t2 = e2->getTime();
    bool greater = (t1 - t2) > 0;
    return greater;
}

// Compares if e1 < e2
bool Event::lessThan(const Event *e1, const Event *e2) {
    return !Event::greaterThan(e1, e2);
}

QList<QString> Event::penaltyTexts = QList<QString>()
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

QString Event::getPenaltyText(int id) {
    return Event::penaltyTexts.value(id, "Unknown penalty");
}
