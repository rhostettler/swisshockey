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

#ifndef PLAYERLIST_H
#define PLAYERLIST_H

#include <QAbstractListModel>
#include <QMap>
#include <QVector>

#include "player.h"

class PlayerList : public QAbstractListModel
{
    Q_OBJECT

    private:
        QMap<quint32, Player *> mPlayers;
        QMap<quint8, quint32> mJerseyNumbers;   // Jersey number <-> player ID
        QVector<quint32> mPlayerIndices;        // Linear index <-> player ID

    protected:
        QHash<int, QByteArray> roleNames() const override;

    public:
        explicit PlayerList(QObject *parent = nullptr);

        // Getters / setters
        Player *getPlayer(quint32 playerId);
        Player *getPlayerByJerseyNumber(quint8 jerseyNumber);

        // Basic functionality:
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
#if 0
        void sort(int column = 0, Qt::SortOrder order = Qt::DescendingOrder);
        void clear(void);
#endif

    public slots:
        void insert(Player *player);
};

#endif // PLAYERLIST_H
