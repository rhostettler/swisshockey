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

#ifndef JSONDECODER_H
#define JSONDECODER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>

// Wrapper to provide a unique decoder interface to the client
class JsonDecoder : public QObject {
    Q_OBJECT

    public:
        explicit JsonDecoder(QObject *parent = 0);
        QMap<QString, QVariant> decode(QByteArray json);

    signals:

    public slots:

};

#endif // JSONDECODER_H
