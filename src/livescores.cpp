#include "livescores.h"

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>

#include <QTextStream>
#include <QObjectList>

#include "gamedata.h"
#include "logger.h"
#include "config.h"

LiveScores::LiveScores(QObject *parent) : QObject(parent) {
    // Create data lists and setup the data source, then run an initial query
    // N.B.: I could maybe use the insert/update facility of the QAbstractItemModel but then the data source would have to know about the model again. We'll se how we'll handle this.
    this->nla = new GamedayData(this);
    this->dataSource = new SIHFDataSource(this);
    connect(this->dataSource, SIGNAL(gameSummaryUpdated(QVariantMap)), this->nla, SLOT(updateData(QVariantMap)));
    this->dataSource->update();

    // Create the notifier
    this->notifier = new Notifier(nla);
    this->notifier->disableNotifications();

    // QML Viewer
    this->viewer = new QmlApplicationViewer();
    this->viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    this->viewer->setMainQmlFile(QLatin1String("qml/main.qml"));

    // Show the view
    this->viewer->showExpanded();

    // Set data sources, connect to signals, and add an event filter to catch
    // switches between foreground and background.
    this->viewer->rootContext()->setContextProperty("listData", nla);
    QObject *rootObject = viewer->rootObject();
    connect(rootObject, SIGNAL(viewChanged(QString)), this, SLOT(updateView(QString)));
    rootObject->installEventFilter(this);

    // TODO: Use this code to show info in the info banner.
/*    QVariant msg = "Hello from C++";
    QMetaObject::invokeMethod(rootObject, "showInfo", Q_ARG(QVariant, msg));*/

    // Create a timer that periodically fires to update the data, defaults to 5
    Config& config = Config::getInstance();
    int updateInterval = config.getValue("updateInterval", 5).toInt();
    this->timer = new QTimer(this);
    this->timer->setSingleShot(false);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateData()));
    this->timer->start(updateInterval*60*1000);
}

// TODO: Re-implement / rework when details view is reworked
void LiveScores::updateView(QString id) {
    this->current = nla->getGame(id);

    if(this->current != NULL) {
        // Set the game id in the totomat & force update
        this->dataSource->setGameId(id);
        this->dataSource->queryStats();

        // Q: can we start the BusyIndicator here?

        // Get the context since we'll be invoking it a couple of times
        QDeclarativeContext *context = viewer->rootContext();

        // Set the info for the page top
        context->setContextProperty("detailstotalscore", this->current->getTotalScore());
        context->setContextProperty("detailshometeamId", this->current->getHometeamId());
        context->setContextProperty("detailsawayteamId", this->current->getAwayteamId());
        context->setContextProperty("detailsperiodsscore", this->current->getPeriodsScore());
        //context->setContextProperty("", );
        // TODO: Here we implement stuff like the game location, refs, spectators

        // Here we set the correct data source, then that's it
        context->setContextProperty("gameEventsData", this->current);
    }
}

// Observe the focus state of the app (foreground / background) and set the
// internal state accordingly
bool LiveScores::eventFilter(QObject* obj, QEvent* event) {
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

    // We don't mind other event handlers handling the events, so return false.
    return false;
}

// Updates the data when the timer fires
void LiveScores::updateData() {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "LiveScores::updateData() called for a data update.");

    // Query the website and update
    // TODO: I think I want to split this into updateSummaries() and updateDetails() ? I have to think about what's the most generic way?
    // Maybe provide an overloaded update() function that takes an argument and if the argument is set, the update is for a specific game?
    this->dataSource->update();
    //this->queryStats();  % Disabled until re-implemented
}

LiveScores::~LiveScores(void) {
    // Remove the ones that are not deleted automagically
    delete viewer;
    delete notifier;
}
