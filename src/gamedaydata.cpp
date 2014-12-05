#include "gamedaydata.h"
#include <QDebug>

GamedayData::GamedayData(QObject *parent) : QAbstractListModel(parent) {
    // Initialize the different data roles
    QHash<int, QByteArray> roles;
    roles[HometeamRole] = "hometeam";
    roles[AwayteamRole] = "awayteam";
    roles[TotalScoreRole] = "totalscore";
    roles[PeriodsScoreRole] = "periodsscore";
    roles[GameStatusRole] = "gamestatus";
    roles[GameIdRole] = "gameid";
    setRoleNames(roles);

    // Initialize the game statuses
    this->gameStatuses.insert(0, "Not Started");
    this->gameStatuses.insert(1, "First Period");
    this->gameStatuses.insert(2, "End of First");
    this->gameStatuses.insert(3, "Second Period");
    this->gameStatuses.insert(4, "End of Second");
    this->gameStatuses.insert(5, "Third Period");
    this->gameStatuses.insert(6, "End of Third");
    this->gameStatuses.insert(7, "Extra");
    this->gameStatuses.insert(8, "Shootout");
    this->gameStatuses.insert(9, "Final (unofficial)");
    this->gameStatuses.insert(10, "Final (unofficial, extra)");
    this->gameStatuses.insert(11, "Final (unofficial, shootout)");
    this->gameStatuses.insert(12, "Final Result");

    // Set the date to <empty>
    this->date = "";
}

void GamedayData::updateGames(QString date, QVariantList data) {
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

    // Add or update each game in the list
    QListIterator<QVariant> iter(data);
    while(iter.hasNext()) {
        // Get the game...
        QVariantMap game = iter.next().toMap();
        qulonglong key = game["gameid"].toULongLong();

        if(this->games.contains(key)) {
            // The game is already in the list, hence, we simply update it with
            // the new data
            this->games[key]->updateGame(game);
        } else {
            // The game couldn't be found in the list so we simply add a new one
            // For that, we need to call beginInsertRows() and endInsertRows()
            // so that the ListView gets notified about the new content.
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            this->games.insert(key, new GameData(game, this));
            this->gameIndices.append(key);
            endInsertRows();
        }

        // Check if game in list has changed, if so, emit signal
        if(this->games[key]->hasChanged()) {
            QModelIndex index = createIndex(this->gameIndices.indexOf(key), 0);
            emit dataChanged(index, index);
        }
    }
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

#if 0
    qDebug() << "GamedayData::data() called. Index: " << index.row() << ", "
             << index.column();
#endif

    // Find the requested item
    qulonglong key = this->gameIndices[index.row()];

    switch(role) {
        case HometeamRole:
            data = this->games[key]->getHometeam();
            break;

        case AwayteamRole:
            data = this->games[key]->getAwayteam();
            break;

        case TotalScoreRole:
            data = this->games[key]->getTotalScore();
            break;

        case PeriodsScoreRole:
            data = this->games[key]->getPeriodsScore(1).append(", ").append(
                        this->games[key]->getPeriodsScore(2)
                    ).append(", ").append(
                        this->games[key]->getPeriodsScore(3)
                    );
            break;

        case GameStatusRole:
            data = this->gameStatuses[this->games[key]->getGameStatus()];
            break;

        case GameIdRole:
            data = QString::number(key);
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
