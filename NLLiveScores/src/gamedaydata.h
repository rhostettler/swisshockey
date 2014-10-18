#ifndef GAMEDAYDATA_H
#define GAMEDAYDATA_H

#include <QAbstractListModel>

#include "gamedata.h"

class GamedayData : public QAbstractListModel {
    Q_OBJECT

    private:
        QMap<int, GameData *> games;
        QList<int> gameIndices;
        QString date;

    public:
        enum GameRoles {
            HometeamRole = Qt::UserRole + 1,
            AwayteamRole,
            TotalScoreRole,
            PeriodsScoreRole,
            GameStatus
        };

        explicit GamedayData(QObject *parent = 0);

        // date: date of the gameday, data: the list of games
        void updateGames(QString date, QVariantList data);

        // implementations of interface QAbstractListModel
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    signals:

    public slots:
    
};

#endif // GAMEDAYDATA_H
