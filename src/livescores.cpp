#include "livescores.h"

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>

#include <QTextStream>
#include <QObjectList>

#include "gamedata.h"

LiveScores::LiveScores(QObject *parent) : QObject(parent) {
    // Create data lists and totomat object
    this->nla = new GamedayData(this);
    this->totomat = new Totomat(this);
    this->totomat->setData(nla);

    // Connect the notifier
    this->notifier = new Notifier(nla);

    // QML Viewer
    this->viewer = new QmlApplicationViewer();
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/main.qml"));

    // Show the view
    viewer->showExpanded();

    // Set data sources and connect to signals
    viewer->rootContext()->setContextProperty("listData", nla);
    QObject *rootObject = viewer->rootObject();
    connect(rootObject, SIGNAL(viewChanged(QString)), this, SLOT(updateView(QString)));
}

void LiveScores::updateView(QString id) {
    this->current = nla->getGame(id);

    if(this->current != NULL) {
        // Set the game id in the totomat & force update
        this->totomat->setGameId(id);
        this->totomat->queryStats();

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

LiveScores::~LiveScores(void) {
    // Remove the ones that are not deleted automagically
    delete viewer;
    delete notifier;
}
