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

#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QAbstractListModel>
#include <QVector>

#include "event.h"

class EventList : public QAbstractListModel {
    Q_OBJECT

    private:
        QVector<Event *> mEvents;

    protected:
        QHash<int, QByteArray> roleNames() const override;

    public:
        explicit EventList(QObject *parent = nullptr);

        // Header:
    //    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        // Basic functionality:
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        void sort(int column = 0, Qt::SortOrder order = Qt::DescendingOrder);
        void clear(void);

    public slots:
        void insert(Event *event);
        //void remove(Event *event);
};

#endif // EVENTLIST_H
