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
