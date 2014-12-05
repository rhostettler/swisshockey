#include <QtGui/QApplication>
#include <MComponentData>
#include "livescores.h"

//Q_DECL_EXPORT int main(int argc, char *argv[])
int main(int argc, char *argv[]) {
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    MComponentData::createInstance(argc, argv);

    // Set the application name
    app->setApplicationName("Live Scores");

    // Create a mediator that generates the UI and connects all the necessary
    // signals, etc.
    LiveScores *livescores = new LiveScores();

    // Run the app
    return app->exec();
}
