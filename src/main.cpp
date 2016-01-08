#include <QtGui/QApplication>
#include <MComponentData>
#include <QTextCodec>

#include "livescores.h"
#include "logger.h"

//Q_DECL_EXPORT int main(int argc, char *argv[])
int main(int argc, char *argv[]) {
    // Initialize the application
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    MComponentData::createInstance(argc, argv);
    app->setApplicationName("Live Scores");

    // Make sure UTF-8 is used internally
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    // Initialize the logger
    Logger& logger = Logger::getInstance();
    logger.setLogfile("/home/user/nl-live-scores.log");
    logger.setLevel(Logger::DEBUG);

    // Create a controller that generates the UI and connects all the necessary
    // signals, etc.
    LiveScores *livescores = new LiveScores();
    //app->connect(app.data(), SIGNAL(focusChanged()), livescores, SLOT(toggleFocus(QWidget *, QWidget *)));

    // Run the app
    int exitcode = app->exec();

    // Close the log
    logger.close();

    return exitcode;
}
