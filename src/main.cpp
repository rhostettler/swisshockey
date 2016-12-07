// Platform specific modules
#ifdef PLATFORM_SFOS
    #ifdef QT_QML_DEBUG
        #include <QtQuick>
    #endif

    #include <QGuiApplication>
    #include <sailfishapp.h>
    #include <QStandardPaths>
    #include <QDir>
#else
    #include <QtGui/QApplication>
    #include <MComponentData>
#endif

// Qt Modules
#include <QTextCodec>

// Local modules
#include "livescores.h"
#include "logger.h"
#include "config.h"

int main(int argc, char *argv[]) {
#ifdef PLATFORM_SFOS
    QGuiApplication *app = SailfishApp::application(argc, argv);
#else
    // Initialize the application
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    MComponentData::createInstance(argc, argv);
#endif
    app->setApplicationName("Swiss Ice Hockey");

#ifndef PLATFORM_SFOS
    // TODO: Need to fix that for SFOS
    // Make sure UTF-8 is used internally    
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    // Initialize the logger
    // TODO: Maybe we coult move this into the LiveScores object?
    Config& config = Config::getInstance();
    Logger& logger = Logger::getInstance();
#ifdef PLATFORM_SFOS
    QDir datapath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if(!datapath.exists()) {
        datapath.mkpath(".");
    }
    logger.setLogfile(datapath.canonicalPath() + "/debug.log");
#else
    logger.setLogfile("/home/user/swisshockey.log");                            // TODO: Update path (make generic)
#endif
    //logger.setLevel(config.getValue("loglevel", Logger::ERROR).toInt());  // TODO: Re-enable
    //logger.setLevel(Logger::DEBUG);
    logger.setLevel(Logger::ERROR);

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
