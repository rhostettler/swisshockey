/*
 * Copyright 2014-present Roland Hostettler
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

#include "player.h"
#include "logger.h"

Player::Player(qulonglong teamId, quint32 id, QObject *parent) : QObject(parent), mTeamId(teamId), mPlayerId(id) {
    mPosition = POSITION_UNDEFINED;
    for(int iStat = 0; iStat < STATS_LEN; iStat++) {
        mStats.append("0");
    }
}

quint32 Player::getPlayerId() const {
    return mPlayerId;
}

void Player::setName(QString firstName, QString lastName) {
    mFirstName = firstName;
    mLastName = lastName;
}

QString Player::getName() const {
    return QString(mFirstName.at(0)) + ". " + mLastName;
}

void Player::setJerseyNumber(quint8 jerseyNumber) {
    mJerseyNumber = jerseyNumber;
}

quint8 Player::getJerseyNumber() const {
    return mJerseyNumber;
}

void Player::setLineNumber(quint8 lineNumber) {
    mLineNumber = lineNumber;
}

quint8 Player::getLineNumber(void) const {
    return mLineNumber;
}

void Player::setPosition(quint8 position) {
    mPosition = position;
    // TODO: Add safegaurds such that the position is valid.
}

quint8 Player::getPosition(void) const {
    return mPosition;
}

QString Player::getPositionString(void) const {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Position is " + QString::number(mPosition)));
    return PositionStrings.at(mPosition);
}

void Player::setStat(int statId, QString value) {
    mStats[statId] = value;
    emit statsChanged();
}

QString Player::getStatsString(void) const {
    QString text;
    if(mPosition == POSITION_GK) {
        text.append(
            "GA: " + mStats[STATS_GK_GA]
            + ", SVS: " + mStats[STATS_GK_SVS]
            + ", SVS%: " + mStats[STATS_GK_SVSP]
        );
    } else {
        text.append(
            "G: " + mStats[STATS_GOALS]
            + ", A: " + mStats[STATS_ASSISTS]
            + ", SOG: " + mStats[STATS_SOG]
            + ", PIM: " + mStats[STATS_PIM]
//            + ", FO%: " + mStats[STATS_FO]
        );
    }

    return text;
}

// Compares if p1 > p2
bool Player::greaterThan(const Player *p1, const Player *p2) {
    quint8 p1Line = p1->getLineNumber();
    quint8 p1Position = p1->getPosition();
    quint8 p2Line = p2->getLineNumber();
    quint8 p2Position = p2->getPosition();

    bool greater = (p1Line > p2Line) || (p1Line == p2Line && p1Position > p2Position);
    return greater;
}

// Compares if p1 < p2
bool Player::lessThan(const Player *p1, const Player *p2) {
    return !Player::greaterThan(p1, p2);
}

bool Player::operator ==(Player const &other) const {
    return (mPlayerId == other.getPlayerId());
}

bool Player::operator !=(Player const &other) const {
    return !(*this == other);
}

QList<QString> Player::PositionStrings = QList<QString>()
    << QString("")
    << QString("GK")
    << QString("LD")
    << QString("RD")
    << QString("LW")
    << QString("RW")
    << QString("C");
