/*
 * Copyright 2014-2017 Roland Hostettler
 *
 * This file is part of swisshockey.
 *
 * swisshockey is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * swisshockey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * swisshockey. If not, see http://www.gnu.org/licenses/.
 */

// Platform specific modules
#ifdef QT_QML_DEBUG
    #include <QtQuick>
#endif

#include <QGuiApplication>
#include <sailfishapp.h>
#include <QStandardPaths>
#include <QDir>

// Qt Modules
//#include <QTextCodec>

// Local modules
#include "livescores.h"
#include "logger.h"
#include "config.h"

int main(int argc, char *argv[]) {
    QGuiApplication *app = SailfishApp::application(argc, argv);
    app->setApplicationName(APP_NAME);

#if 0
    // TODO: Check if this is still needed
    // Make sure UTF-8 is used internally    
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    // Initialize the logger
    // TODO: Maybe we coult move this into the LiveScores object?
    Config& config = Config::getInstance();
    Logger& logger = Logger::getInstance();
    QDir datapath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if(!datapath.exists()) {
        datapath.mkpath(".");
    }
    logger.setLogfile(datapath.canonicalPath() + "/debug.log");
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

    // Free the memory and exit
    delete livescores;
    return exitcode;
}
