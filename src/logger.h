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

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>

// Simple logger using the singleton pattern according to
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class Logger {
    public:
        // Log-level definition
        enum LOGLEVEL {
            ERROR = 1,
            INFO,
            WARN,
            DEBUG
        };

        static Logger& getInstance();

        // Delete the constructor and '=' operator to avoid spawning (C++ 11)
#if 0
        Logger(Logger const&) = delete;
        void operator=(Logger const&) = delete;
#endif

        // Logging facility functions
        void setLogfile(QString);
        void close();
        void setLevel(int);
        void log(int, QString);
        void dump(QString, QString);

    private:
        Logger() {};

        // Redefine the constructor and '='-operator to avoid spawning (C++ 03)
        Logger(Logger const&);
        void operator=(Logger const&);

        // The current log-level
        int loglevel;

        // Logfile
        QFile* logfile;
        QTextStream* stream;
};

#endif // LOGGER_H
