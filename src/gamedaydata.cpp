#include "gamedaydata.h"

#include <QDate>

#include "logger.h"

GamedayData::GamedayData(QObject *parent) : QAbstractListModel(parent) {
    // Initialize the different data roles that can be used by the ListView
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

    // Set the date to <empty>
    this->date = QDate::currentDate().toString("yyyy-MM-dd");;

    // Signal mapper acts as a proxy between the GameData -> GamedayData -> outside world
    this->signalMapper = new QSignalMapper(this);
    connect(this->signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(gamedataChanged(const QString &)));
}

void GamedayData::updateGames(QString date, QVariantMap data) {
    // Check if we're updating the current game day or if we're given the
    // data for a new day, in order not to accumulate a lot of old games when
    // the app is kept open for over a day.
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

    // TODO: I should replace the qulonglong with QStrings throughout the code.
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

        // Listen to the scoreChanged()- and statusChanged()-signals to know
        // when we need to notify the view through the dataChanged()-signal.
        // We have to do this through the SignalMapper because GameData's
        // signals don't take arguments but we need to be able to identify
        // the sender in GamedayData (and the views).
        connect(this->games[key], SIGNAL(scoreChanged()), this->signalMapper, SLOT(map()));
        connect(this->games[key], SIGNAL(statusChanged()), this->signalMapper, SLOT(map()));
        signalMapper->setMapping(this->games[key], QString::number(key));
    }
}

// A re-implementation of the updateGames (?). Simply forward the request for now.
void GamedayData::updateData(QVariantMap data) {
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    this->updateGames(date, data);
}

void GamedayData::gamedataChanged(const QString & key) {
    QModelIndex index = createIndex(this->gameIndices.indexOf(key.toLongLong()), 0);
    emit dataChanged(index, index);
}

GameData* GamedayData::getGame(QString id) {
    return this->games[id.toLongLong()];
}

// Impelementation of QAbstractListModel follows below
// Returns the number of rows in the list
int GamedayData::rowCount(const QModelIndex &parent) const {
    return this->games.count();
}

// Returns the data requested by the view
QVariant GamedayData::data(const QModelIndex &index, int role) const {
    QVariant data;

    // Map from the ListView index to the game key
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
            data = this->games[key]->getGameStatusText();
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
    return QVariant();
}

/* EOF */
