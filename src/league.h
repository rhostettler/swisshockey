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

#ifndef LEAGUE_H
#define LEAGUE_H

#include <QObject>
#include <QString>

class League : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString id READ getId)
    Q_PROPERTY(QString abbreviation READ getAbbreviation)
    Q_PROPERTY(QString name READ getName)

    private:
        uint mId;
        QString mAbbreviation;
        QString mName;

    public:
        explicit League(uint id, QString abbreviation, QString name, QObject *parent = 0);
        QString getId(void);
        QString getAbbreviation(void);
        QString getName(void);

    signals:

    public slots:
};

Q_DECLARE_METATYPE(League *)

#endif // LEAGUE_H
