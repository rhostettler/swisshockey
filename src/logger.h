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
            ERROR,
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
