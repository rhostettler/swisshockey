#ifndef GAMEDAYDATA_H
#define GAMEDAYDATA_H

#include <QAbstractListModel>
#include <QSignalMapper>

#include "gamedata.h"

class GamedayData : public QAbstractListModel {
    Q_OBJECT

    private:
        QMap<qulonglong, GameData *> games;
        QList<qulonglong> gameIndices;
        QString date;
        QSignalMapper *signalMapper;

    public:
        explicit GamedayData(QObject *parent = 0);

        // date: date of the gameday, data: the list of games
        void updateGames(QString date, QVariantMap data);

        GameData * getGame(QString id);

        // implementations of interface QAbstractListModel
        enum GameRoles {
            HometeamRole = Qt::UserRole + 1,
            HometeamIdRole,
            AwayteamRole,
            AwayteamIdRole,
            TotalScoreRole,
            PeriodsScoreRole,
            GameStatusRole,
            GameIdRole,
            LeagueRole
        };

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    signals:

    public slots:
        void updateData(QVariantMap data);
        void gamedataChanged(qulonglong key);
    
};

#endif // GAMEDAYDATA_H
