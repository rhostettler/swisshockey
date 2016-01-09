#include "livescores.h"

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>

#include <QTextStream>
#include <QObjectList>

#include "gamedata.h"
#include "logger.h"

LiveScores::LiveScores(QObject *parent) : QObject(parent) {
    // Create data lists and setup the data source, then run an initial query
    // N.B.: I could maybe use the insert/update facility of the QAbstractItemModel but then the data source would have to know about the model again. We'll se how we'll handle this.
    this->nla = new GamedayData(this);
    this->dataSource = new SIHFDataSource(this);
    connect(this->dataSource, SIGNAL(gameSummaryUpdated(QVariantMap)), this->nla, SLOT(updateData(QVariantMap)));
    this->dataSource->update();

    // Create the notifier
    this->notifier = new Notifier(nla);
    //this->notifier->disableNotifications();  // TODO: Since the FG/BG switching doesn't work, I'll keep the notifier enabled for now.

    // QML Viewer
    this->viewer = new QmlApplicationViewer();
    this->viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    this->viewer->setMainQmlFile(QLatin1String("qml/main.qml"));

    // Show the view
    this->viewer->showExpanded();

    // Set data sources and connect to signals
    this->viewer->rootContext()->setContextProperty("listData", nla);
    QObject *rootObject = viewer->rootObject();
    connect(rootObject, SIGNAL(viewChanged(QString)), this, SLOT(updateView(QString)));

    // Create a timer that periodically fires to update the data. Update
    // interval is set to 5 mins (5*60*1000 ms).
    this->timer = new QTimer(this);
    this->timer->setSingleShot(false);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateData()));
    this->timer->start(30000);
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

// TODO: Re-implement / rework when notifications is reworked
// Observe the focus state of the app (foreground / background) and set the
// internal state accordingly
void LiveScores::toggleFocus(QWidget *old, QWidget *now) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "LiveScores::toggleFocus(): Called.");

    if(old == NULL) {
        // If old is NULL we came to focus
        logger.log(Logger::DEBUG, "LiveScores::toggleFocus(): App came to foreground.");
        this->notifier->disableNotifications();
    } else if(now == NULL) {
        // If now is NULL, the app went to the background
        logger.log(Logger::DEBUG, "LiveScores::toggleFocus(): App went to background.");
        this->notifier->enableNotifications();
    } else {
        logger.log(Logger::DEBUG, "LiveScores::toggleFocus(): Something is wrong.");
    }
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
