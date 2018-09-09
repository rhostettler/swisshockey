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

#ifdef PLATFORM_SFOS
    #include <mlite5/mgconfitem.h>
#else
    #include <gconfitem.h>
#endif

#include "config.h"

Config& Config::getInstance() {
    // Create an instance upon the first call that is guaranteed to be destroyed
    // upon deletion of the object
    static Config instance;
    return instance;
}

QVariant Config::getValue(QString key, QVariant def) {
    QString gconfPath = "/apps/NLLiveScores/settings/";
    QString gconfKey = gconfPath.append(key);
#ifdef PLATFORM_SFOS
    MGConfItem item(gconfKey);
#else
    GConfItem item(gconfKey);
#endif
    return item.value(def);
}
