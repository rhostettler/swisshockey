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

#include "jsondecoder.h"

JsonDecoder::JsonDecoder(QObject *parent) : QObject(parent) {
}

QMap<QString, QVariant> JsonDecoder::decode(QByteArray json) {
    QMap<QString, QVariant> map;
    QJsonDocument document = QJsonDocument::fromJson(json);

    if(document.isObject()) {
        map = document.object().toVariantMap();
    } else if(document.isArray()) {
        //map = document.array().to
    }

    return map;
}
