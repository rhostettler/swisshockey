/*
 *  Copyright 2017 Roland Hostettler
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

#ifdef PLATFORM_SFOS
    #include <QQmlContext>
    #include <QQuickItem>
#else
    #include <QDeclarativeView>
    #include <QDeclarativeContext>
    #include <QGraphicsObject>
#endif

#include "logger.h"
#include "config.h"

// SailfishApp::main() will display "qml/template.qml", if you need more
// control over initialization, you can use:
//
//   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
//   - SailfishApp::createView() to get a new QQuickView * instance
//   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
//
// To display the view, call "show()" (will show fullscreen on device).
LiveScores::LiveScores(QObject *parent) : QObject(parent) {
    // Create the data store and setup the data provider
    mDataStore = new GamedayData(this);
    mDataSource = new SIHFDataSource(this);
    connect(mDataSource, SIGNAL(gameSummaryUpdated(QVariantMap)), mDataStore, SLOT(updateData(QVariantMap)));

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
    // TODO: Enable on SFOS
#ifdef PLATFORM_SFOS
    // Not yet implemented for SFOS
#else
    this->notifier = new Notifier(dataStore);
    this->notifier->disableNotifications();
#endif

    // Load and show the QML
#ifdef PLATFORM_SFOS
    mQmlViewer = SailfishApp::createView();
    mQmlViewer->setSource(SailfishApp::pathTo("sailfishos/harbour-swisshockey.qml"));
    mQmlViewer->show();
#else
    mQmlViewer = new QmlApplicationViewer();
    mQmlViewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    mQmlViewer->setMainQmlFile(QLatin1String("harmattan/main.qml"));
    mQmliewer->showExpanded();
#endif

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
        logger.log(Logger::DEBUG, "LiveScores::LiveScores(): Couldn't find the 'overviewPage' QML object, updates in progress will not be shown.");
    } else {
        connect(mDataSource, SIGNAL(updateStarted()), overviewPage, SLOT(startUpdateIndicator()));
        connect(mDataSource, SIGNAL(updateFinished()), overviewPage, SLOT(stopUpdateIndicator()));
    }

    // Trigger an update after all the GUI signals have been connected.
    // currentId is "NULL" by default.
    mDataSource->update(mSelectedGameId);

    // Create a timer that periodically fires to update the data, defaults to 5 mins
    Config& config = Config::getInstance();
    int updateInterval = config.getValue("updateInterval", 2).toInt();
    mUpdateTimer = new QTimer(this);
    mUpdateTimer->setSingleShot(false);
    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    mUpdateTimer->start(updateInterval*60*1000);

#if 0
    // TODO: Use this code to show the info banner from C++ (Harmattan).
    QVariant msg = "Hello from C++";
    QMetaObject::invokeMethod(rootObject, "showInfo", Q_ARG(QVariant, msg));
#endif
}

// Called when the user switches from the summaries to the details view
// TODO: Maybe we should trigger a busy indicator here too?
void LiveScores::updateView(QString id) {
    GameData *game = mDataStore->getGame(id);
    mSelectedGameId = id;

    if(game != NULL) {
        // Set the game id in the totomat & force update
        connect(mDataSource, SIGNAL(gameDetailsUpdated(QList<GameEvent *>, QVariantList)), game, SLOT(updateEvents(QList<GameEvent *>, QVariantList)));
        mDataSource->getGameDetails(id);

        // Set the details data models
#ifdef PLATFORM_SFOS
        QQmlContext *context = mQmlViewer->rootContext();
#else
        QDeclarativeContext *context = viewer->rootContext();
#endif
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
#ifndef PLATFORM_SFOS
    delete notifier;
#endif
}
