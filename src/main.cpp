
// Platform specific modules
#ifdef PLATFORM_SFOS
    #ifdef QT_QML_DEBUG
        #include <QtQuick>
    #endif

    #include <QGuiApplication>
    #include <sailfishapp.h>
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

/*
// SailfishApp::main() will display "qml/template.qml", if you need more
// control over initialization, you can use:
//
//   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
//   - SailfishApp::createView() to get a new QQuickView * instance
//   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
//
// To display the view, call "show()" (will show fullscreen on device).

return SailfishApp::main(argc, argv);
*/

//Q_DECL_EXPORT int main(int argc, char *argv[])
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
    logger.setLogfile("/home/user/swisshockey.log");                            // TODO: Update path
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
