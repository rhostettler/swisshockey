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

Player::Player(qulonglong teamId, quint32 id, QObject *parent) : QObject(parent), mTeamId(teamId), mPlayerId(id) {
    //mPlayerId = id;
    //mTeamId = teamId;
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

void Player::setPosition(quint8 position) {
    mPosition = position;
    // TODO: Add safegaurds such that the position is valid.
}

bool Player::operator ==(Player const &other) const {
    return (mPlayerId == other.getPlayerId());
}

bool Player::operator !=(Player const &other) const {
    return !(*this == other);
}
