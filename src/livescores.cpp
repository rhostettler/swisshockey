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

#include "livescores.h"

#include <QQmlContext>
#include <QQuickItem>

#include "logger.h"
#include "config.h"
#include "notifier.h"

// SailfishApp::main() will display "qml/template.qml", if you need more
// control over initialization, you can use:
//
//   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
//   - SailfishApp::createView() to get a new QQuickView * instance
//   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
//
// To display the view, call "show()" (will show fullscreen on device).
LiveScores::LiveScores(QObject *parent) : QObject(parent) {
    //mAppVersion.append(APP_VERSION);
    mAppName.append(APP_NAME);

    // Create the data store and setup the data provider
    mDataStore = new GamedayData(this);
    mDataSource = new SIHFDataSource(this);
    connect(mDataSource, SIGNAL(summaryUpdated(QVariantMap)), mDataStore, SLOT(updateData(QVariantMap)));

    // Create a filter for the league, acts as a proxy between the view and the
    // data store
    mLeagueFilter = new QSortFilterProxyModel(this);
    mLeagueFilter->setFilterRole(GamedayData::LeagueRole);
    mLeagueFilter->setDynamicSortFilter(true);
    mLeagueFilter->setFilterKeyColumn(0);  // We only have the 0-column
    mLeagueFilter->setFilterRegExp(".*");
    mLeagueFilter->setSourceModel(mDataStore);

    // Create the notifier, disabled by default (enabled automatically when the
    // app is brought to the background)
    // TODO: Consider having on-screen notification banner when in foreground
    mNotifier = new Notifier(mDataStore, this);
//    mNotifier->disableNotifications();

    // Load and show the QML
    mQmlViewer = SailfishApp::createView();
    mQmlViewer->setSource(SailfishApp::pathTo("sailfishos/harbour-swisshockey.qml"));
    mQmlViewer->show();

    // Get the leagues
    mDataSource->getLeagues(&mLeaguesList);

    // Connect the QML and the C++ bits, and add an event filter to catch
    // switches between foreground and background.
    mQmlViewer->rootContext()->setContextProperty("listData", this->mLeagueFilter);
    mQmlViewer->rootContext()->setContextProperty("leagueList", QVariant::fromValue(mLeaguesList));
    QObject *rootObject = mQmlViewer->rootObject();
    connect(rootObject, SIGNAL(viewChanged(QString)), this, SLOT(updateView(QString)));
    connect(rootObject, SIGNAL(leagueChanged(QString)), this, SLOT(updateLeague(QString)));
    connect(rootObject, SIGNAL(updateTriggered()), this, SLOT(updateData()));  // Manually trigger update
    rootObject->installEventFilter(this);

    // Connect the update*()-signals to the update indicator
    QObject *overviewPage = rootObject->findChild<QObject*>("overviewPage");
    if(overviewPage == 0) {
        Logger& logger = Logger::getInstance();
        logger.log(Logger::DEBUG, QString(Q_FUNC_INFO).append("LiveScores::LiveScores(): Couldn't find the 'overviewPage' QML object, updates in progress will not be shown."));
    } else {
        connect(mDataSource, SIGNAL(updateStarted()), overviewPage, SLOT(startUpdateIndicator()));
        connect(mDataSource, SIGNAL(updateFinished()), overviewPage, SLOT(stopUpdateIndicator()));
    }

    // Trigger an update after all the GUI signals have been connected.
    // currentId is "NULL" by default.
    mDataSource->update(mSelectedGameId);

    // Create a timer that periodically fires to update the data, defaults to 5 mins
    Config& config = Config::getInstance();
    int updateInterval = config.getValue("updateInterval", 0.5).toInt();
    mUpdateTimer = new QTimer(this);
    mUpdateTimer->setTimerType(Qt::PreciseTimer);
    mUpdateTimer->setSingleShot(false);
    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    mUpdateTimer->start(updateInterval*60*1000);

#if 0
    // TODO: Use this code to show the info banner from C++ (Harmattan).
    QVariant msg = "Hello from C++";
    QMetaObject::invokeMethod(rootObject, "showInfo", Q_ARG(QVariant, msg));
#endif
}

// App name
QString LiveScores::getAppName() const {
    return mAppName;
}

// App version
QString LiveScores::getAppVersion() const {
    return mAppVersion;
}

// Called when the user switches from the summaries to the details view
// TODO: Maybe we should trigger a busy indicator here too?
void LiveScores::updateView(QString id) {
    mSelectedGameId = id;
    mNotifier->setGameId(id);

    GameData *game = mDataStore->getGame(id);
    if(game != NULL) {
        // Set the game id in the totomat & force update
        connect(mDataSource, SIGNAL(eventsUpdated(QList<GameEvent *>)), game, SLOT(updateEvents(QList<GameEvent *>)));
        connect(mDataSource, SIGNAL(playersUpdated(QList<Player *>)), game, SLOT(updateRosters(QList<Player *>)));
        mDataSource->getGameDetails(id);

        // Set the details data models
        QQmlContext *context = mQmlViewer->rootContext();
        context->setContextProperty("gameDetailsData", game);
        context->setContextProperty("gameEventsData", game);
        // TODO: Set home & away team rosters here
    }
}

// Update the filter to the selected league
void LiveScores::updateLeague(QString leagueId) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "LiveScores::updateLeague(): Changing league filter to " + leagueId);
    if(!leagueId.compare("0")) {
        mLeagueFilter->setFilterRegExp(".*");
    } else {
        mLeagueFilter->setFilterRegExp("^" + leagueId + "$");
    }
}

// Observe the focus state of the app (foreground / background) and set the
// internal state accordingly
bool LiveScores::eventFilter(QObject* obj, QEvent* event) {
#ifndef PLATFORM_SFOS
    Logger& logger = Logger::getInstance();

    switch(event->type()) {
        case QEvent::WindowActivate:
            this->notifier->disableNotifications();
            this->notifier->clearNotifications();
            logger.log(Logger::DEBUG, "LiveScores::eventFilter(): Switched to foreground, notifications disabled.");
            break;

        case QEvent::WindowDeactivate:
            this->notifier->enableNotifications();
            logger.log(Logger::DEBUG, "LiveScores::eventFilter(): Switched to background, notifications enabled.");
            break;

        default:
            break;
    }
#endif

    // We don't mind other event handlers handling the events, so return false.
    return false;
}

// Updates the data when the timer fires or when triggered by the user
void LiveScores::updateData() {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "LiveScores::updateData(): called for a data update.");
    mDataSource->update(mSelectedGameId);
}

LiveScores::~LiveScores(void) {
    // Remove the ones that are not deleted automagically
    delete mQmlViewer;
    delete mNotifier;
}
