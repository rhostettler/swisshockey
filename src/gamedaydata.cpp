#include "gamedaydata.h"
#include <QDebug>

GamedayData::GamedayData(QObject *parent) : QAbstractListModel(parent) {
    // Initialize the different data roles
    QHash<int, QByteArray> roles;
    roles[HometeamRole] = "hometeam";
    roles[HometeamIdRole] = "hometeamId";
    roles[AwayteamRole] = "awayteam";
    roles[AwayteamIdRole] = "awayteamId";
    roles[TotalScoreRole] = "totalscore";
    roles[PeriodsScoreRole] = "periodsscore";
    roles[GameStatusRole] = "gamestatus";
    roles[GameIdRole] = "gameid";
    roles[LeagueRole] = "league";

    setRoleNames(roles);

    // Initialize the game statuses
    this->gameStatuses.insert(0, "Not Started");
    this->gameStatuses.insert(1, "First Period");
    this->gameStatuses.insert(2, "End of First");
    this->gameStatuses.insert(3, "Second Period");
    this->gameStatuses.insert(4, "End of Second");
    this->gameStatuses.insert(5, "Third Period");
    this->gameStatuses.insert(6, "End of Third");
    this->gameStatuses.insert(7, "Overtime");
    this->gameStatuses.insert(8, "Shootout");
    this->gameStatuses.insert(9, "Final (unofficial)");
    this->gameStatuses.insert(10, "Final (unofficial, overtime)");
    this->gameStatuses.insert(11, "Final (unofficial, shootout)");
    this->gameStatuses.insert(12, "Final");

    // Set the date to <empty>
    this->date = "";
}

void GamedayData::updateGames(QString date, QVariantMap data) {
#if 0 // TODO: This code will be re-enabled later on I think
    // Check if the we're updating the current game day or if we're given the
    // data for a new day
    if(this->date.compare(date)) {
        // Clear the whole list
        this->date = date;
        beginResetModel();
        this->games.clear();
        this->gameIndices.clear();
        endResetModel();
    } else {
        // NOP.
    }
#endif

#if 0
    // Add or update each game in the list
    QListIterator<QVariant> iter(data);
    while(iter.hasNext()) {
        // Get the game...
        QVariantMap game = iter.next().toMap();
#endif

        ////////

        qulonglong key = data["gameId"].toULongLong();

        if(this->games.contains(key)) {
            // The game is already in the list, hence, we simply update it with
            // the new data
            this->games[key]->updateSummary(data);
        } else {
            // The game couldn't be found in the list so we simply add a new one
            // For that, we need to call beginInsertRows() and endInsertRows()
            // so that the ListView gets notified about the new content.
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            this->games.insert(key, new GameData(data, this));
            this->gameIndices.append(key);
            endInsertRows();
        }

        // Check if game in list has changed, if so, emit signal
        if(this->games[key]->hasChanged()) {
            QModelIndex index = createIndex(this->gameIndices.indexOf(key), 0);
            emit dataChanged(index, index);
        }
#if 0
    }
#endif
}

// A re-implementation of the updateGames (?). Simply forward the request for now.
void GamedayData::updateData(QVariantMap data) {
    this->updateGames("", data);
}

GameData * GamedayData::getGame(QString id) {
    return this->games[id.toLongLong()];
}

// Returns the number of rows in the list
int GamedayData::rowCount(const QModelIndex &parent) const {
    return this->games.count();
}

// Returns the data requested by the view
QVariant GamedayData::data(const QModelIndex &index, int role) const {
    QVariant data;

    // Find the requested item
    qulonglong key = this->gameIndices[index.row()];

    switch(role) {
        case HometeamRole:
            data = this->games[key]->getHometeam();
            break;

        case HometeamIdRole:
            data = this->games[key]->getHometeamId();
            break;

        case AwayteamRole:
            data = this->games[key]->getAwayteam();
            break;

        case AwayteamIdRole:
            data = this->games[key]->getAwayteamId();
            break;

        case TotalScoreRole:
            data = this->games[key]->getTotalScore();
            break;

        case PeriodsScoreRole:
            data = this->games[key]->getPeriodsScore();
            break;

        case GameStatusRole:
            data = this->gameStatuses[this->games[key]->getGameStatus()];
            break;

        case GameIdRole:
            data = QString::number(key);
            break;

        case LeagueRole:
        data = this->games[key]->getLeague();
            break;

        default:
            break;
    }

    return data;
}

// Returns the header
QVariant GamedayData::headerData(int section, Qt::Orientation orientation, int role) const {
    qDebug() << "Header data called";
    return QVariant();
}
