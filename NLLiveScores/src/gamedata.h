#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QMap>
#include "gamedata.h"

class GameData : public QObject {
    Q_OBJECT

    private:
        // Home- and away team
        QString hometeam;
        QString awayteam;

        // Score
        QMap<QString, QString> score;

        // Status
        int status;

        // Flag that indicates whether the game has changed or not
        bool changed;

        // Method that parses the team name and removes special characters
        QString parseTeam(QString team);

    public:
        explicit GameData(QVariantMap game, QObject *parent = 0);
        void updateGame(QVariantMap game);
        bool hasChanged(void);

        QString getHometeam();
        QString getAwayteam();
        QString getTotalScore();
        QString getPeriodsScore(int period);
        int getGameStatus();

    signals:

    public slots:
};

#endif // GAMEDATA_H
