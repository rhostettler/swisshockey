#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QMap>
#include <QAbstractListModel>

#include "gamedata.h"
#include "gameevent.h"

//class GameData : public QObject {
class GameData : public QAbstractListModel {
    Q_OBJECT

    private:
        QString gameId;

        // Home- and away team
        QString hometeam;
        qulonglong hometeamId;
        QString awayteam;
        qulonglong awayteamId;

        // Score
        QMap<QString, QString> score;

        // List of events
        QList<GameEvent *> events;

        // List of players
        QMap<qint32, QString> players;

        // Status
        int status;

        // Flag that indicates whether the game has changed or not
        bool scoreChanged;
        bool statusChanged;

        // Method that parses the team name and removes special characters
        QString parseTeam(QString team);

    public:
        explicit GameData(QVariantMap data, QObject *parent = 0);
        void updateGame(QVariantMap data);
        void updateEvents(QVariantMap gameInfo, QVariantList goals, QVariantList fouls, QVariantMap players);
        void updatePlayerList(QVariantMap players);
        void updateGoals(QVariantList goals);
        void updateFouls(QVariantList fouls);
        bool hasChanged(void);
        bool hasChanged(QString type);

        QString getHometeam();
        QString getAwayteam();
        QString getTotalScore();
        QString getPeriodsScore(int period);
        QString getPeriodsScore();
        int getGameStatus();

        // implementations of interface QAbstractListModel
        enum EventRoles {
            TeamRole = Qt::UserRole + 1,
            TimeRole,
            PlayerRole,
            AdditionalInfoRole,
            EventRole
        };

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        static QString parsePlayerName(QString name);

    signals:

    public slots:
};

#endif // GAMEDATA_H
