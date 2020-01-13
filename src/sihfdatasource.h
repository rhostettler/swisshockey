/*
 * Copyright 2014-2017 Roland Hostettler
 *
 * This file is part of swisshockey.
 *
 * swisshockey is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * swisshockey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * swisshockey. If not, see http://www.gnu.org/licenses/.
 */

#ifndef SIHFDATASOURCE_H
#define SIHFDATASOURCE_H

#include <QString>
#include <QVariantList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "datasource.h"
#include "gamedaydata.h"
#include "jsondecoder.h"
#include "league.h"
#include "player.h"

class SIHFDataSource : public DataSource {
    Q_OBJECT

    private:
        QNetworkAccessManager *mNetworkManager;
        QNetworkReply *mSummariesReply;
        QNetworkReply *mDetailsReply;
        JsonDecoder *mJSONDecoder;

        // Private helper functions
        void parseGame(QVariantList data);
        QList<Player *> parsePlayers(QVariantList data);
        void parsePlayers(GameData *game, QVariantList data);
        QList<GameEvent *> parseGoals(QVariantList data);
        QList<GameEvent *> parsePenalties(QVariantList data);
        QList<GameEvent *> parseGoalkeepers(QVariantList data);
        QList<GameEvent *> parseShootout(QVariantList data);

        static QMap<uint, League *> mLeaguesMap;

        static const QString SCORES_URL;
        static const QString DETAILS_URL;

        enum GAME_SUMMARY_FIELDS {
            GS_LEAGUE_NAME = 0,
            GS_TIME,
            GS_HOMETEAM,
            GS_AWAYTEAM,
            GS_PHASE,
            GS_TOTALSCORE,
            GS_PERIODSSCORE,
            GS_OTINDICATOR,
            GS_META,
            GS_DETAILS,
            GS_BROADCASTS,
            GS_LENGTH
        };

    public:
        explicit SIHFDataSource(GamedayData *gamesList, QObject *parent = 0);
        void update(QString id);
        void getGameSummaries(void);
        void getGameDetails(QString gameId);

        // League stuff
        void getLeagues(QList<QObject *> *leagueList);
        QString getLeagueId(QString abbreviation);
        static const QMap<uint, League *> initLeagueList(void);

    signals:

    public slots:
        void parseGameSummaries();
        void parseGameDetails();
        void handleNetworkError(QNetworkReply::NetworkError error);
};

#endif // SIHFDATASOURCE_H
