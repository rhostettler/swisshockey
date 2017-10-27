/*
 * Copyright 2017 Roland Hostettler
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

#ifndef CONFIG_H
#define CONFIG_H

#include <QVariant>
#include <QString>

// Config class to access GConf in a consistent way
class Config {
    public:
        static Config& getInstance();

        QVariant getValue(QString key, QVariant def);

    private:
        Config() {};

        // Redefine the constructor and '='-operator to avoid spawning (C++ 03)
        Config(Config const&);
        void operator=(Config const&);

};

#endif // CONFIG_H
