#include <QStringList>
#include <QDebug>
#include "gamedata.h"

// Instantate and initialize the game
GameData::GameData(QVariantMap game, QObject *parent) : QObject(parent) {
    // Get the game ID
    this->gameId = game["gameid"].toString();

    // Get the teams for this game
    QStringList teams = game["text"].toString().split(":");
    this->hometeam = parseTeam(teams[0].remove(teams[0].length()-1, 1));
    this->awayteam = parseTeam(teams[1].remove(0, 1));

    // Initialize the score
    this->score["total"] = QString("0:0");
    this->score["first"] = QString("0:0");
    this->score["second"] = QString("0:0");
    this->score["third"] = QString("0:0");

    // Forward the game and update the result
    updateGame(game);

    // Reset the changed flag to prevent notifications on application startup
    this->scoreChanged = false;
    this->statusChanged = false;
}

QString GameData::parseTeam(QString team) {
    team.replace("&egrave;", "e");
    return team;
}

// Update the game score
void GameData::updateGame(QVariantMap game) {
    // Store the total score before the update
    QString oldScore = this->score["total"];
    int oldStatus = this->status;

    // Get the score from the game map
    QVariantMap newScore = game["result"].toMap();

    // Update score per period
    QMapIterator<QString, QVariant> iter(newScore);
    while(iter.hasNext()) {
        iter.next();
        QVariantMap tmp = iter.value().toMap();
        this->score[iter.key()] = tmp["goals"].toString();
    }

    // Update game status
    this->status = game["status"].toInt();

    // Check if the total score changed and set the flag
    if(this->score["total"].compare(oldScore)) {
        this->scoreChanged = true;
    }

    // Check if the game status has changed and set the flag
    if(this->status != oldStatus) {
        this->statusChanged = true;
    }

    // Debug info
#if 0
    qDebug() << "Game updated:";
    qDebug() << this->hometeam << " - " << this->awayteam;
    qDebug() << this->score["total"] << "(" << this->score["first"] << ", "
             << this->score["second"] << ", " << this->score["third"] << ")";
    qDebug() << "Status: " << game["status"].toInt();
#endif
}

// Return the status of the game (changed/the same) since last read. Resets the
// flags!
bool GameData::hasChanged(void) {
    bool changed = this->scoreChanged || this->statusChanged;
    this->scoreChanged = false;
    this->statusChanged = false;
    return changed;
}

// Check and reset whether the field "type" changed
bool GameData::hasChanged(QString type) {
    bool changed = false;

    if(type.compare("score")) {
        changed = this->scoreChanged;
        this->scoreChanged = false;
    }

    if(type.compare("status")) {
        changed = this->statusChanged;
        this->statusChanged = false;
    }

    return changed;
}

QString GameData::getHometeam() {
    return this->hometeam;
}

QString GameData::getAwayteam() {
    return this->awayteam;
}

QString GameData::getTotalScore() {
    return this->score["total"];
}

QString GameData::getPeriodsScore(int period) {
    QString key;

    switch(period) {
        case 1:
            key = "first";
            break;
        case 2:
            key = "second";
            break;
        case 3:
            key = "third";
            break;
        default:
            key = "first";
            break;
    }

    return this->score[key];
}

int GameData::getGameStatus() {
    return this->status;
}
