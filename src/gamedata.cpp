#include "gamedata.h"
#include "logger.h"

// The list of game status texts
QStringList GameData::gameStatusTexts = QStringList()
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
GameData::GameData(QVariantMap data, QObject *parent) : QAbstractListModel(parent) {
    // Get the league
    this->league = data["league"].toString();

    // Get the game ID
    this->gameId = data["gameId"].toString();

    // Set the teams for this game
    this->hometeam = data["hometeam"].toString(); // parseTeam(teams[0].remove(teams[0].length()-1, 1));
    this->hometeamId = data["hometeamId"].toLongLong();
    this->awayteam = data["awayteam"].toString(); //parseTeam(teams[1].remove(0, 1));
    this->awayteamId = data["awayteamId"].toLongLong();

    // Forward the rest and update the result
    updateSummary(data);

    // Reset the changed flag to prevent notifications on application startup
    this->m_scoreChanged = false;
    this->m_statusChanged = false;

    // Set the data roles
    //QHash<int, QByteArray> roles;
    this->roles[TeamRole] = "eventteam";
    this->roles[TimeRole] = "eventtime";
    this->roles[PlayerRole] = "eventplayer";
    this->roles[AdditionalInfoRole] = "eventinfo";
    this->roles[EventRole] = "eventtext";
    this->roles[EventSubtextRole] = "eventsubtext";
    //setRoleNames(roles);
}

// Update the game score
void GameData::updateSummary(QVariantMap data) {
    // Store the total score before the update
#if 0
    QString oldScore = this->score["total"];
#endif
    QMap<QString, QString> oldScore = this->score;
    int oldStatus = this->status;

    // Update score per period
    QVariantMap newScore = data["score"].toMap();
    this->score["first"] = newScore.value("first", "-:-").toString();
    this->score["second"] = newScore.value("second", "-:-").toString();
    this->score["third"] =  newScore.value("third", "-:-").toString();
    if(newScore.contains("overtime")) {
        this->score["overtime"] =  newScore.value("overtime", "-:-").toString();
    }
    this->score["total"] = newScore["total"].toString();

    // Update game status
    this->status = data["status"].toInt();

    // Check if the total score changed and trigger an update
#if 0
    if(this->score["total"].compare(oldScore)) {
#endif
    if(this->score != oldScore) {
        // TODO: I should also fire a scoreChanged() when a new period starts
        // for updating the details view
        // => Should be fixed now, but there's no way of knowing really until friday
        emit scoreChanged();
    }

    // Check if the game status has changed and signal it
    if(this->status != oldStatus) {
        emit statusChanged();
    }
}

// TODO: For simplicity, we update the parsing functions here for now, we'll
// move to the generic solution later on.
void GameData::updateEvents(QVariantList goals, QVariantList fouls, QVariantList players) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "GameData::updateEvents(): Updating game events.");

    // Clear all events
    beginResetModel();
    this->events.clear();
    endResetModel();

    // Update the list of events (if there are any)
    if(goals.size() + fouls.size() > 0) {
        //beginInsertRows(QModelIndex(), rowCount(), rowCount());
        // Parse the players, goals, and fouls
        this->updatePlayerList(players);
        this->updateGoals(goals);
        this->updateFouls(fouls);

        // Sort the events
        qSort(this->events.begin(), this->events.end(), GameEvent::greaterThan);

        // End insert/remove rows
        //this->endInsertRows();

        logger.log(Logger::DEBUG, "GameData::updateEvents(): Added " + QString::number(this->players.size()) + " players and " + QString::number(this->events.size()) + " events.");
    } else {
        logger.log(Logger::DEBUG, "GameData::updateEvents(): No game events to add.");
    }
}

// Parses the player list and adds them to the local list of players where we
// have a player license <=> player name map
void GameData::updatePlayerList(QVariantList players) {
    QListIterator<QVariant> iterator(players);

    while(iterator.hasNext()) {
        QVariantMap player = iterator.next().toMap();
        quint32 playerId = player.value("id").toUInt();
        QString playerName = player.value("fullName").toString();
        int index = playerName.lastIndexOf(" ");
        QString lastName = playerName.left(index);
        QString firstName = playerName.at(index+1);
        QString name = firstName + ". " + lastName;
        this->players.insert(playerId, name);
    }
}

// Parse the list of goals and add an event for each goal
void GameData::updateGoals(QVariantList goals) {
    QListIterator<QVariant> iterator(goals);
    int cumulativeHomeScore = 0;
    int cumulativeAwayScore = 0;

    // Iterate through the goals, the oldest is first in the list
    while(iterator.hasNext()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        QVariantMap goal = iterator.next().toMap();
        QString time = goal.value("time").toString();
        QString player = this->players.value(goal.value("scorerLicenceNr").toUInt());
        QString additionalInfo = this->players.value(goal.value("assist1LicenceNr").toUInt(), "");
        QString assist2 = this->players.value(goal.value("assist2LicenceNr").toUInt(), "");
        if(assist2.compare("") != 0) {
            additionalInfo = additionalInfo + ", " + assist2;
        }

        if(goal.value("teamId").toLongLong() == this->hometeamId) {
            cumulativeHomeScore += 1;
        } else {
            cumulativeAwayScore += 1;
        }
        QString score = QString::number(cumulativeHomeScore) + ":" + QString::number(cumulativeAwayScore);

        // Finally, add the goal as a new event
        this->events.append(new GameEvent(time, player, additionalInfo, score));
        endInsertRows();
    }
}

// Parse the list of fouls and add an event for each foul
void GameData::updateFouls(QVariantList fouls) {
    QListIterator<QVariant> iterator(fouls);

    while(iterator.hasNext()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        QVariantMap foul = iterator.next().toMap();
        QString time = foul.value("time").toString();
        QString player = this->players.value(foul.value("playerLicenceNr").toUInt(), "");
        QString additionalInfo = GameEvent::getPenaltyText(foul.value("id").toInt());
        QString penalty = foul.value("minutes").toString() + "'";
        this->events.append(new GameEvent(time, player, additionalInfo, penalty));
        endInsertRows();
    }
}

// Return the status of the game (changed/the same) since last read. Resets the
// flags!
// DEPRECATED
bool GameData::hasChanged(void) {
    bool changed = this->m_scoreChanged || this->m_statusChanged;
    this->m_scoreChanged = false;
    this->m_statusChanged = false;
    return changed;
}

// Check and reset whether the field "type" changed
// DEPRECATED
bool GameData::hasChanged(QString type) {
    bool changed = false;

    if(type.compare("score")) {
        changed = this->m_scoreChanged;
        this->m_scoreChanged = false;
    }

    if(type.compare("status")) {
        changed = this->m_statusChanged;
        this->m_statusChanged = false;
    }

    return changed;
}

QString GameData::getLeague() {
    return this->league;
}

QString GameData::getHometeam() {
    return this->hometeam;
}

QString GameData::getHometeamId() {
    return QString::number(this->hometeamId);
}

QString GameData::getAwayteam() {
    return this->awayteam;
}

QString GameData::getAwayteamId() {
    return QString::number(this->awayteamId);
}

QString GameData::getTotalScore() {
    return this->score["total"];
}

QString GameData::getPeriodsScore() {
    QString score = this->score.value("first") + ", " +
        this->score.value("second") + ", " + this->score.value("third");

    if(this->score.contains("overtime")) {
        score.append(", " + this->score.value("overtime"));
    }

    return score;
}

int GameData::getGameStatus() {
    return this->status;
}

QString GameData::getGameStatusText() {
    return GameData::gameStatusTexts.value(this->status, "Unknown status");
}

// Implementation of the QAbstractListModel methods
// Returns the number of rows in the list
int GameData::rowCount(const QModelIndex &parent) const {
    return this->events.count();
}

// Returns the data requested by the view
QVariant GameData::data(const QModelIndex &index, int role) const {
    QVariant data;
    int key = index.row();

    switch(role) {
        case TeamRole:
            // TODO: Needs to be implemented in GameEvent
            //data = this->events[key]->getTeamId();
            break;

        case TimeRole:
            data = this->events[key]->getTime();
            break;

        case PlayerRole:
            data = this->events[key]->getPlayer();
            break;

        case AdditionalInfoRole:
            data = this->events[key]->getAdditionalInfo();
            break;

        case EventRole:
            data = this->events[key]->getEvent();
            break;

        case EventSubtextRole:
            // TODO: This is meant to be used with "EQ", "PP1", etc. in the future.
            //       Needs to be implemented in the parser.
            data = this->events[key]->getEventSubtext();
            break;

        default:
            break;
    }

    return data;
}

// Returns the header
QVariant GameData::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

// Role names for QML
QHash<int, QByteArray> GameData::roleNames() const {
    return this->roles;
}

/* EOF */
