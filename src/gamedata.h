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
        QString league;

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
#if 0
        QString parseTeam(QString team);
#endif

    public:
        explicit GameData(QVariantMap data, QObject *parent = 0);
        void updateSummary(QVariantMap data);
        void updatePlayerList(QVariantList players);
        void updateGoals(QVariantList goals);
        void updateFouls(QVariantList fouls);
        bool hasChanged(void);
        bool hasChanged(QString type);

        QString getLeague();
        QString getHometeam();
        QString getHometeamId();
        QString getAwayteam();
        QString getAwayteamId();
        QString getTotalScore();
#if 0
        QString getPeriodsScore(int period);
#endif
        QString getPeriodsScore();
        int getGameStatus();

        // implementations of interface QAbstractListModel
        enum EventRoles {
            TeamRole = Qt::UserRole + 1,
            TimeRole,
            PlayerRole,
            AdditionalInfoRole,
            EventRole,
            EventSubtextRole
        };

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        static QString parsePlayerName(QString name);

    signals:

    public slots:
        void updateEvents(QVariantList goals, QVariantList fouls, QVariantList players);
};

#endif // GAMEDATA_H
