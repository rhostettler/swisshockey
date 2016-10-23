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
    this->dataStore = new GamedayData(this);
    this->dataSource = new SIHFDataSource(this);
    connect(this->dataSource, SIGNAL(gameSummaryUpdated(QVariantMap)), this->dataStore, SLOT(updateData(QVariantMap)));

    // Create a filter for the league, acts as a proxy between the view and the
    // data store
    this->filter = new QSortFilterProxyModel(this);
    this->filter->setFilterRole(GamedayData::LeagueRole);
    this->filter->setDynamicSortFilter(true);
    this->filter->setFilterKeyColumn(0);  // We only have the 0-column
    this->filter->setFilterRegExp("NL A");
    this->filter->setSourceModel(this->dataStore);

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
    this->viewer = SailfishApp::createView();
    this->viewer->setSource(SailfishApp::pathTo("qml/sailfishos/harbour-swisshockey.qml"));  // TODO: Adjust once the qml is sorted out
    this->viewer->show();
#else
    this->viewer = new QmlApplicationViewer();
    this->viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    this->viewer->setMainQmlFile(QLatin1String("harmattan/main.qml"));
    this->viewer->showExpanded();
#endif

    // Connect the QML and the C++ bits, and add an event filter to catch
    // switches between foreground and background.
    this->viewer->rootContext()->setContextProperty("listData", this->filter);
    QObject *rootObject = viewer->rootObject();
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
        connect(dataSource, SIGNAL(updateStarted()), overviewPage, SLOT(startUpdateIndicator()));
        connect(dataSource, SIGNAL(updateFinished()), overviewPage, SLOT(stopUpdateIndicator()));
    }

    // Trigger an update after all the GUI signals have been connected.
    // currentId is "NULL" by default.
    this->dataSource->update(this->currentId);

    // Create a timer that periodically fires to update the data, defaults to 5 mins
    Config& config = Config::getInstance();
    int updateInterval = config.getValue("updateInterval", 2).toInt();
    this->timer = new QTimer(this);
    this->timer->setSingleShot(false);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateData()));
    this->timer->start(updateInterval*60*1000);

#if 0
    // TODO: Use this code to show the info banner from C++.
    QVariant msg = "Hello from C++";
    QMetaObject::invokeMethod(rootObject, "showInfo", Q_ARG(QVariant, msg));
#endif
}

// Called when the user switches from the summaries to the details view
// TODO: Maybe we should trigger a busy indicator here too?
void LiveScores::updateView(QString id) {
    GameData *game = dataStore->getGame(id);
    this->currentId = id;

    if(this->current != NULL) {
        // Set the game id in the totomat & force update
        connect(this->dataSource, SIGNAL(gameDetailsUpdated(QVariantList, QVariantList, QVariantList)), game, SLOT(updateEvents(QVariantList, QVariantList, QVariantList)));
        this->dataSource->queryStats(id);

        // Get the context since we'll be invoking it a couple of times
#ifdef PLATFORM_SFOS
        QQmlContext *context = viewer->rootContext();
#else
        QDeclarativeContext *context = viewer->rootContext();
#endif
        context->setContextProperty("gameDetailsData", game);
        context->setContextProperty("gameEventsData", game);
    }
}

// Update the filter to the selected league
void LiveScores::updateLeague(QString league) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "LiveScores::updateLeague(): Changing league filter to " + league);
    this->filter->setFilterRegExp(league);
}

// Observe the focus state of the app (foreground / background) and set the
// internal state accordingly
bool LiveScores::eventFilter(QObject* obj, QEvent* event) {
    Logger& logger = Logger::getInstance();

#ifndef PLATFORM_SFOS
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

// Updates the data when the timer fires
void LiveScores::updateData() {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "LiveScores::updateData(): called for a data update.");

    // Query the website and update
    // TODO: I think I want to split this into updateSummaries() and updateDetails() ? I have to think about what's the most generic way?
    // Maybe provide an overloaded update() function that takes an argument and if the argument is set, the update is for a specific game?
    this->dataSource->update(this->currentId);
    //this->queryStats();  % Disabled until re-implemented
}

LiveScores::~LiveScores(void) {
    // Remove the ones that are not deleted automagically
    delete viewer;
#ifndef PLATFORM_SFOS
    delete notifier;
#endif
}
