/*
 * Copyright 2014-present Roland Hostettler
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

#include "game.h"
#include "logger.h"

// The list of game status texts
QStringList Game::GameStatusTexts = QStringList()
    << QString("Not Started")    // 0
    << QString("First Period")
    << QString("End of First")
    << QString("Second Period")
    << QString("End of Second")
    << QString("Third Period")   // 5
    << QString("End of Third")
    << QString("Overtime")
    << QString("Shootout")
    << QString("Final (unofficial)")
    << QString("Final (unofficial, overtime)") // 10
    << QString("Final (unofficial, shootout)")
    << QString("Final");

// Initialize the game
Game::Game(QString gameId, QObject *parent) : QObject(parent) {
    // Store game ID
    mGameId = gameId;
}

QString Game::getGameId(void){
    return mGameId;
}

void Game::setLeague(QString leagueId) {
    mLeagueId = leagueId;
}

QString Game::getLeague() {
    return this->mLeagueId;
}

void Game::setDateTime(QString time) {
    mStartTime = time;
}

void Game::setHometeam(QString id, QString name) {
    mHometeamId = id.toLongLong();
    mHometeamName = name;
}

QString Game::getHometeam() {
    return mHometeamName;
}

QString Game::getHometeamId() {
    return QString::number(this->mHometeamId);
}

void Game::setAwayteam(QString id, QString name) {
    mAwayteamId = id.toLongLong();
    mAwayteamName = name;
}

QString Game::getAwayteam() {
    return this->mAwayteamName;
}

QString Game::getAwayteamId() {
    return QString::number(this->mAwayteamId);
}

void Game::setScore(QMap<QString, QString> score) {
    // Update the score and trigger a signal if it changed
    QMap<QString, QString> oldScore = mScore;
    mScore = score;
    if(mScore["total"] != oldScore["total"] && oldScore["total"] != "-:-") {
        emit scoreChanged();
    }
}

QString Game::getTotalScore() {
    return this->mScore["total"];
}

QString Game::getPeriodsScore() {
    QString score = mScore.value("first") + ", " + mScore.value("second") + ", " + mScore.value("third");

    if(mScore.contains("overtime")) {
        score.append(", " + this->mScore.value("overtime"));
    }

    return score;
}

void Game::setStatus(int status) {
    // Update the game status and trigger a signal if it changed
    int oldStatus = mGameStatus;
    mGameStatus = status;
    if(mGameStatus != oldStatus) {
        emit statusChanged();
    }
}

int Game::getStatus() {
    return this->mGameStatus;
}

QString Game::getStatusString() {
    QString text;
    if(this->mGameStatus == 0) {
        text = "Starts " + this->mStartTime;
    } else {
        text = Game::GameStatusTexts.value(this->mGameStatus, "Unknown status");
    }

    return text;
}

EventList *Game::getEventList(void) {
    return &mEventList;
}

PlayerList *Game::getHometeamRoster() {
    return &mHometeamRoster;
}

PlayerList *Game::getAwayteamRoster() {
    return &mAwayteamRoster;
}
