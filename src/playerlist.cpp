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

#include <algorithm>

#include "playerlist.h"
#include "logger.h"

PlayerList::PlayerList(QObject *parent) : QAbstractListModel(parent) {
}

Player *PlayerList::getPlayer(quint32 playerId) {
    return mPlayers.value(playerId, nullptr);
}

Player *PlayerList::getPlayerByJerseyNumber(quint8 jerseyNumber){
    quint32 playerId = mJerseyNumbers.value(jerseyNumber);
    return mPlayers.value(playerId, nullptr);
}

int PlayerList::rowCount(const QModelIndex &parent) const {
#if 0
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if(parent.isValid()) {
        return 0;
    }
#endif
    return mPlayers.size();
}

QVariant PlayerList::data(const QModelIndex &index, int /* role */) const {
    if(!index.isValid()) {
        return QVariant();
    }
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append(": Item requested, item number is " + QString::number(index.row())) + ", list size is " + QString::number(mPlayers.size()));

    quint32 playerId = mPlayerIndices[index.row()];
    Player *player = mPlayers.value(playerId);
    return QVariant::fromValue(player);
}

void PlayerList::insert(Player *player) {
    quint32 playerId = player->getPlayerId();
    if(!mPlayers.contains(playerId)) {
        beginInsertRows(QModelIndex(), 0, 0);
        mPlayers.insert(playerId, player);
        mJerseyNumbers.insert(player->getJerseyNumber(), playerId);
        mPlayerIndices.append(playerId);
        endInsertRows();
    }
}

#if 0
void PlayerList::sort(int column, Qt::SortOrder order) {
    layoutAboutToBeChanged();
    if(order == Qt::AscendingOrder) {
        std::sort(mPlayers.begin(), mPlayers.end(), Player::lessThan);
    } else {
        std::sort(mPlayers.begin(), mPlayers.end(), Player::greaterThan);
    }
    layoutChanged();
}
#endif

void PlayerList::clear(void) {
    beginResetModel();
    mPlayers.clear();
    mJerseyNumbers.clear();
    mPlayerIndices.clear();
    endResetModel();
}

QHash<int, QByteArray> PlayerList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "player";
    return roles;
}
