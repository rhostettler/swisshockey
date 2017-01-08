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
    this->roles[TeamRole] = "eventteam";
    this->roles[TimeRole] = "eventtime";
    this->roles[PlayerRole] = "eventplayer";
    this->roles[AdditionalInfoRole] = "eventinfo";
    this->roles[EventRole] = "eventtext";
    this->roles[EventSubtextRole] = "eventsubtext";
#ifndef PLATFORM_SFOS
    setRoleNames(this->roles);
#endif
}

// TODO: This has to be re-written such that we take a Game(Data) object and compare the two.
// Update the game score
void GameData::updateSummary(QVariantMap data) {
    // Store the total score before the update
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

    // Update game starting time and status
    this->startTime = data["time"].toString();
    this->status = data["status"].toInt();

    // Check if the total score changed and trigger an UI update
    if(this->score != oldScore) {
        emit scoreChanged();
    }

    // Check if the game status has changed and signal it
    if(this->status != oldStatus) {
        emit statusChanged();
    }
}

// TODO: Event handling is OK now, players not yet.
void GameData::updateEvents(QList<GameEvent *> events, QVariantList players) {
    Logger& logger = Logger::getInstance();
    logger.log(Logger::DEBUG, "GameData::updateEvents(): Updating game events.");

    // Clear all events
    beginResetModel();
    this->events.clear();
    endResetModel();

    // Update the list of events (if there are any)
    QListIterator<GameEvent *> iter(events);
    while(iter.hasNext()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        this->events.append(iter.next());
        endInsertRows();

        // Parse the players, goals, and fouls
        this->updatePlayerList(players);
    }
    logger.log(Logger::DEBUG, "GameData::updateEvents(): Added " + QString::number(this->players.size()) + " players and " + QString::number(this->events.size()) + " events.");

    // Sort the events
    layoutAboutToBeChanged();
    qSort(this->events.begin(), this->events.end(), GameEvent::greaterThan);
    layoutChanged();
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
    QString text;
    if(this->status == 0) {
        text = "Starts " + this->startTime;
    } else {
        text = GameData::gameStatusTexts.value(this->status, "Unknown status");
    }

    return text;
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
    GameEvent *event = this->events[key];

    switch(role) {
        case TeamRole:
            data = event->getTeam();
            break;

        case TimeRole: {
                float tmp = event->getTime();
                quint32 minutes = (int)tmp/60;
                quint32 seconds = (int)(tmp-minutes*60);
                QString str;
                str.sprintf("%02u:%02u", minutes, seconds);
                data = str;
            }
            break;

        case PlayerRole: {
                quint32 id = event->getPlayer();
                data = this->players.value(id, "");
            }
            break;

        case AdditionalInfoRole: {
                int type = event->getType();
                switch(type) {
                    case GameEvent::GOAL: {
                            QString text = this->players.value(event->getPlayer(GameEvent::FIRST_ASSIST), "");
                            QString tmp = this->players.value(event->getPlayer(GameEvent::SECOND_ASSIST), "");
                            if(tmp != "") {
                                text.append(", " + tmp);
                            }
                            data = text;
                        }
                        break;

                    case GameEvent::PENALTY:
                        data = GameEvent::getPenaltyText(event->getPenalty());
                        break;

                    case GameEvent::GOALKEEPER_IN:
                        data = "Goalkeeper in";
                        break;

                    case GameEvent::GOALKEEPER_OUT:
                        data = "Goalkeeper out";
                        break;

                    case GameEvent::PENALTY_SHOT:
                        data = "Goalkeeper: " + this->players.value(event->getPlayer(GameEvent::GOALKEEPER), "");
                        break;

                    default:
                        break;
                }
            }
            break;

        case EventRole:
            data = event->getEvent();
            break;

        case EventSubtextRole:
            data = event->getEventSubtext();
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
#ifdef PLATFORM_SFOS
QHash<int, QByteArray> GameData::roleNames() const {
    return this->roles;
}
#endif
