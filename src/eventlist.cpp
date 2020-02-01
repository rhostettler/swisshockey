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

#include "eventlist.h"

EventList::EventList(QObject *parent) : QAbstractListModel(parent) {
}

#if 0
QVariant EventList::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}
#endif

int EventList::rowCount(const QModelIndex &parent) const {
#if 0
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
#endif
    return mEvents.size();
}

QVariant EventList::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    Event *event = mEvents.at(index.row());
    return QVariant::fromValue(event);
}

void EventList::insert(Event *event) {
    beginInsertRows(QModelIndex(), 0, 0);
    mEvents.append(event);
    endInsertRows();
}

#if 0
void EventList::remove(Event *event) {
    for (int i = 0; i < mEvents.size(); ++i) {
    if (mEvents.at(i) == item) {
        beginRemoveRows(QModelIndex(), i, i);
        mEvents.remove(i);
        endRemoveRows();
        break;
        }
    }
}
#endif

void EventList::sort(int column, Qt::SortOrder order) {
    layoutAboutToBeChanged();
    if(order == Qt::AscendingOrder) {
        std::sort(mEvents.begin(), mEvents.end(), Event::lessThan);
    } else {
        std::sort(mEvents.begin(), mEvents.end(), Event::greaterThan);
    }
    layoutChanged();
}

void EventList::clear(void) {
    beginResetModel();
    mEvents.clear();
    endResetModel();
}

QHash<int, QByteArray> EventList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "event";
    return roles;
}
