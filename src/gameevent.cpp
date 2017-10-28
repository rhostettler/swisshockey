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

#include "gameevent.h"
#include "logger.h"

GameEvent::GameEvent(int type) {
    this->type = type;
}

int GameEvent::getType(void) {
    return this->type;
}

void GameEvent::setTime(QString time) {
    QStringList split = time.split(":");
    if(split.size() == 2) {
        this->time = 60*split[0].toFloat() + split[1].toFloat();
    } else {
        this->time = 0;
    }
}

float GameEvent::getTime(void) const {
    return this->time;
}

void GameEvent::setTeam(qlonglong team) {
    this->team = team;
}

qlonglong GameEvent::getTeam(void) {
    return this->team;
}

void GameEvent::addPlayer(int role, quint32 playerId){
    this->players[role] = playerId;
}

void GameEvent::setScore(QString score, QString type) {
    if(this->type == GameEvent::GOAL) {
        this->score = score;
        this->scoreType = type;
    }
}

void GameEvent::setPenalty(int id, QString type) {
    if(this->type == GameEvent::PENALTY) {
        this->penaltyId = id;
        this->penaltyType = type;
    }
}

int GameEvent::getPenalty(void) {
    return this->penaltyId;
}

void GameEvent::setPenaltyShot(bool scored) {
    this->penaltyShot = scored;
}

quint32 GameEvent::getPlayer(void) const {
    int role = -1;
    switch(this->type) {
        case GameEvent::GOAL:
            role = GameEvent::SCORER;
            break;

        case GameEvent::PENALTY:
            role = GameEvent::PENALIZED;
            break;

        case GameEvent::GOALKEEPER_IN:
            role = GameEvent::GOALKEEPER;
            break;

        case GameEvent::GOALKEEPER_OUT:
            role = GameEvent::GOALKEEPER;
            break;

        case GameEvent::PENALTY_SHOT:
            role = GameEvent::SCORER;
            break;

        default:
            role = -1;
            break;
    }
    return this->players[role];
}

quint32 GameEvent::getPlayer(int role) const {
    return this->players[role];
}

// TODO: These things should be done in the GameData class, really. Kept for
// convenience for now.
QString GameEvent::getAdditionalInfo(void) const {
    QString info("Temporary regression.");
    switch(this->type) {
        case GameEvent::GOAL:
            break;
        case GameEvent::PENALTY:
            break;
        default:
            break;
    }

    return info;
}

QString GameEvent::getEvent(void) const {
    QString event("");
    switch(this->type) {
        case GameEvent::GOAL:
            event = this->score;
            break;
        case GameEvent::PENALTY:
            event = this->penaltyType;
            break;
        case GameEvent::PENALTY_SHOT:
            if(this->penaltyShot) {
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

QString GameEvent::getEventSubtext(void) const {
    QString info;
    if(this->type == GameEvent::GOAL) {
        info = this->scoreType;
    }

    return info;
}

// Compares if e1 > e2
bool GameEvent::greaterThan(const GameEvent *e1, const GameEvent *e2) {
    float t1 = e1->getTime();
    float t2 = e2->getTime();
    bool greater = (t1 - t2) > 0;
    return greater;
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
