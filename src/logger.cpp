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
