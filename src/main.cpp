#include <QtGui/QApplication>
#include <MComponentData>
#include <QTextCodec>

#include "livescores.h"
#include "logger.h"
#include "config.h"

//Q_DECL_EXPORT int main(int argc, char *argv[])
int main(int argc, char *argv[]) {
    // Initialize the application
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    MComponentData::createInstance(argc, argv);
    app->setApplicationName("Swiss Ice Hockey");

    // Make sure UTF-8 is used internally
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    // Initialize the logger
    // TODO: Maybe we coult move this into the LiveScores object?
    Config& config = Config::getInstance();
    Logger& logger = Logger::getInstance();
    logger.setLogfile("/home/user/swisshockey.log");
    logger.setLevel(config.getValue("loglevel", Logger::ERROR).toInt());
    //logger.setLevel(Logger::DEBUG);
    //logger.setLevel(Logger::ERROR);

    // Create a controller that generates the UI and connects all the necessary
    // signals, etc.
    LiveScores *livescores = new LiveScores();

    // Run the app
    int exitcode = app->exec();

    // Close the log
    logger.close();
    delete livescores;

    return exitcode;
}
