#include "livescores.h"

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>

#include <QTextStream>
#include <QObjectList>

#include "gamedata.h"

LiveScores::LiveScores(QObject *parent) : QObject(parent) {
    // Create data lists and setup the data source, then run an initial query
    this->nla = new GamedayData(this);
    this->dataSource = new SIHFDataSource(this);
    this->dataSource->setData(nla);
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

    // Set data sources and connect to signals
    this->viewer->rootContext()->setContextProperty("listData", nla);
    QObject *rootObject = viewer->rootObject();
    connect(rootObject, SIGNAL(viewChanged(QString)), this, SLOT(updateView(QString)));

    // Create a timer and set the update interval to 5 mins
#if 0
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(updateData()));
    this->timer->start(30000);
#endif
}

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
        context->setContextProperty("detailshometeam", this->current->getHometeam());
        context->setContextProperty("detailsawayteam", this->current->getAwayteam());
        context->setContextProperty("detailsperiodsscore", this->current->getPeriodsScore());
        //context->setContextProperty("", );
        // TODO: Here we implement stuff like the game location, refs, spectators

        // Here we set the correct data source, then that's it
        context->setContextProperty("gameEventsData", this->current);
    }
}

// Observe the focus state of the app (foreground / background) and set the
// internal state accordingly
void LiveScores::toggleFocus(QWidget *old, QWidget *now) {
    // If old is NULL we came to focus
    if(old == NULL) {
        this->notifier->disableNotifications();
    }

    // If now is NULL, the app went to the background
    else if(now == NULL) {
        this->notifier->enableNotifications();
    }
}

// Update the data when the timer fires
void LiveScores::updateData() {
    // First, disable the timer
    this->timer->stop();

    // Query the website and update
    this->dataSource->update();
    //this->queryStats();  % Disabled until re-implemented
}

LiveScores::~LiveScores(void) {
    // Remove the ones that are not deleted automagically
    delete viewer;
    delete notifier;
}
