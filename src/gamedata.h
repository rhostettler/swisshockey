#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QVariantMap>
#include <QMap>
#include <QAbstractListModel>

#include "gamedata.h"
#include "gameevent.h"


class GameData : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(QString hometeamId READ getHometeamId)
    Q_PROPERTY(QString awayteamId READ getAwayteamId)
    Q_PROPERTY(QString totalScore READ getTotalScore NOTIFY scoreChanged)
    Q_PROPERTY(QString periodsScore READ getPeriodsScore NOTIFY scoreChanged)
    Q_PROPERTY(int gameStatus READ getGameStatus NOTIFY statusChanged)

    private:
        QString gameId;
        QString league;

        QHash<int, QByteArray> roles;

        // Home- and away team names & IDs
        QString hometeam;
        qulonglong hometeamId;
        QString awayteam;
        qulonglong awayteamId;

        // Score
        QMap<QString, QString> score;

        // List of events
        QList<GameEvent *> events;

        // List of players
        QMap<quint32, QString> players;

        // Status
        int status;

        // Flag that indicates whether the game has changed or not
        bool m_scoreChanged;
        bool m_statusChanged;

        //
        static QStringList gameStatusTexts;

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
        QString getPeriodsScore();
        int getGameStatus();
        QString getGameStatusText();

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
#ifdef PLATFORM_SFOS
        QHash<int, QByteArray> roleNames() const;
#endif

        static QString parsePlayerName(QString name);

    signals:
        void scoreChanged(void);
        void statusChanged(void);

    public slots:
        void updateEvents(QList<GameEvent *> events, QVariantList players);
};

#endif // GAMEDATA_H
