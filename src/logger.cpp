#include "logger.h"

Logger& Logger::getInstance() {
    // Create an instance upon the first call that is guaranteed to be destroyed
    // upon deletion of the object
    static Logger instance;

    // TODO: How can I initialize safely? (log level, fp)

    return instance;
}

void Logger::setLogfile(QString filename) {
    // Open the file for reading
    this->logfile = new QFile(filename);

    if(this->logfile->open(QIODevice::Append)) {
        this->stream = new QTextStream(this->logfile);
    }
}

void Logger::close() {
    // TODO: This is unsafe again!
    // Write everything out
    this->stream->flush();
    delete this->stream;

    // Close the logfile
    this->logfile->close();
    delete this->logfile;
}

void Logger::setLevel(int level) {
    // TODO: Safety checks
    this->loglevel = level;
}

void Logger::log(int level, QString message) {
    // Check if the message is of a type below the current log level and that
    // the logfile is writable
    // TODO: Check if it is writable? That maybe fails above?
    if(level <= this->loglevel && true) {
        *(this->stream) << message << endl;

        // TODO: Add some fancy pants stuff ([$LEVEL], timestamp, etc.)
    } else {
        // Discard the message
    }
}
