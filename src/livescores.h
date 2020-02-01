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

#ifndef LIVESCORES_H
#define LIVESCORES_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QEvent>
#include <QSortFilterProxyModel>
#include <QQuickView>
#include <sailfishapp.h>

#include "sihfdatasource.h"
#include "gamelist.h"
#include "notifier.h"

class LiveScores : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ getAppVersion)
    Q_PROPERTY(QString appName READ getAppName)

    private:
        QString mAppName;
        QString mAppVersion;
        QQuickView *mQmlViewer;
        Notifier *mNotifier;
        GameList *mGamesList;
        QSortFilterProxyModel *mLeagueFilter;
        SIHFDataSource *mDataSource;
        QTimer *mUpdateTimer;
        QString mSelectedGameId;
        QList<QObject *> mLeaguesList;

    public:
        explicit LiveScores(QObject *parent = 0);
        QString getAppName() const;
        QString getAppVersion() const;
        bool eventFilter(QObject *, QEvent *);
        ~LiveScores(void);

    signals:

    public slots:
        void updateData();
        void updateView(QString view);
        void updateLeague(QString leagueId);
};

#endif // LIVESCORES_H
