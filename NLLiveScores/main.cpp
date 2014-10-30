#include <QtGui/QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QString>
#include <MComponentData>
#include <MNotification>

#include <QTextStream>

#include "qmlapplicationviewer.h"
#include "src/totomat.h"
#include "src/gamedaydata.h"
#include "src/notifier.h"

//Q_DECL_EXPORT int main(int argc, char *argv[])
int main(int argc, char *argv[]) {
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    MComponentData::createInstance(argc, argv);

    // Set the application name
    app->setApplicationName("Live Scores");

    // Create data lists and totomat object
    GamedayData *nla = new GamedayData();
    Totomat *totomat = new Totomat();//(QApplication *)&app);
    totomat->setData(nla);

    // Connect the notifier
    Notifier *notifier = new Notifier(nla);

    // QML Viewer
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/NLLiveScores/main.qml"));
    viewer.rootContext()->setContextProperty("listData", nla);
    viewer.showExpanded();

    return app->exec();
}
