#include <QStringList>
#include <QTextStream>
#include "gamedata.h"
#include "logger.h"

// Instantate and initialize the game
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
    this->scoreChanged = false;
    this->statusChanged = false;

    // Set the data roles
    QHash<int, QByteArray> roles;
    roles[TeamRole] = "eventteam";
    roles[TimeRole] = "eventtime";
    roles[PlayerRole] = "eventplayer";
    roles[AdditionalInfoRole] = "eventinfo";
    roles[EventRole] = "eventtext";
    roles[EventSubtextRole] = "eventsubtext";
    setRoleNames(roles);
}

#if 0
// TODO: May be removed?
QString GameData::parseTeam(QString team) {
    team.replace("&egrave;", "e");
    team.replace("", "e");
    return team;
}
#endif

// Update the game score
void GameData::updateSummary(QVariantMap data) {
    // Store the total score before the update
    QString oldScore = this->score["total"];
    int oldStatus = this->status;

    // Get the score from the game map
    QVariantMap newScore = data["score"].toMap();

    // Update score per period
    this->score["first"] = newScore.value("first", "-:-").toString();
    this->score["second"] = newScore.value("second", "-:-").toString();
    this->score["third"] =  newScore.value("third", "-:-").toString();
    if(newScore.contains("overtime")) {
        this->score["overtime"] =  newScore.value("overtime", "-:-").toString();
    }
    this->score["total"] = newScore["total"].toString();

    // Update game status
    this->status = data["status"].toInt();

    // Check if the total score changed and set the flag
    if(this->score["total"].compare(oldScore)) {
        this->scoreChanged = true;
    }

    // Check if the game status has changed and set the flag
    if(this->status != oldStatus) {
        this->statusChanged = true;
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
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        // Parse the players, goals, and fouls
        this->updatePlayerList(players);
        this->updateGoals(goals);
        this->updateFouls(fouls);

        // Sort the events
        qSort(this->events.begin(), this->events.end(), GameEvent::greaterThan);

        // End insert/remove rows
        this->endInsertRows();

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

#if 0
// TODO: That should be obsolete now that we have proper UTF-8 parsing
QString GameData::parsePlayerName(QString name) {
    QMap<QString, QString> lookupTable = QMap<QString, QString>();
    lookupTable.insert(QString::fromUtf8("Ã©"), QString::fromUtf8("é"));
    lookupTable.insert(QString::fromUtf8("Ã´"), QString::fromUtf8("ô"));
    lookupTable.insert(QString::fromUtf8("Ã¯"), QString::fromUtf8("ï"));
    lookupTable.insert(QString::fromUtf8("Ã¼"), QString::fromUtf8("ü"));

    QMapIterator<QString, QString> iterator(lookupTable);
    while(iterator.hasNext()) {
        QString key = iterator.next().key();
        name.replace(key, lookupTable.value(key, ""));
    }

    return name;
}
#endif

// Parse the list of goals and add an event for each goal
void GameData::updateGoals(QVariantList goals) {
    QListIterator<QVariant> iterator(goals);
    int cumulativeHomeScore = 0;
    int cumulativeAwayScore = 0;

    // Iterate through the goals list backwards since the newest is the first
#if 0
    iterator.toBack();
    while(iterator.hasPrevious()) {
        QVariantMap goal = iterator.previous().toMap();
#endif
    while(iterator.hasNext()) {
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
        QString score = QString::number(cumulativeHomeScore).append(":").append(QString::number(cumulativeAwayScore));

        // Finally, add the goal as a new event
        this->events.append(new GameEvent(time, player, additionalInfo, score));
    }
}

// Parse the list of fouls and add an event for each foul
void GameData::updateFouls(QVariantList fouls) {
    QListIterator<QVariant> iterator(fouls);

    while(iterator.hasNext()) {
        QVariantMap foul = iterator.next().toMap();
        QString time = foul.value("time").toString();
        QString player = this->players.value(foul.value("playerLicenceNr").toUInt(), "");
        QString additionalInfo = GameEvent::getPenaltyText(foul.value("id").toInt());
        QString penalty = foul.value("minutes").toString() + "'";
        this->events.append(new GameEvent(time, player, additionalInfo, penalty));
    }
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

#if 0
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

    return this->score.value(key);
}
#endif

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

// implementation of the listmodel
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
            // needs to be implemented
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
            data = this->events[key]->getEventSubtext(); // TODO: A Hack!
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
