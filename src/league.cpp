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

#include "league.h"

League::League(uint id, QString abbreviation, QString name, QObject *parent) : QObject(parent) {
    mId = id;
    mAbbreviation = abbreviation;
    mName = name;
}

QString League::getId(void) {
    return QString::number(mId);
}

QString League::getAbbreviation(void) {
    return mAbbreviation;
}

QString League::getName(void) {
    return mName;
}
