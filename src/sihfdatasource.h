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

class SIHFDataSource : public DataSource {
    Q_OBJECT

    private:
        QNetworkAccessManager *nam;
        QNetworkReply *summariesReply;
        QNetworkReply *detailsReply;
        JsonDecoder *decoder;
        QString gameId; // TODO: Should this be here?

        // Private helper functions
        QVariantMap parseGame(QVariantList indata);
        QList<GameEvent *> parseGoals(QVariantList data);
        QList<GameEvent *> parsePenalties(QVariantList data);
        QList<GameEvent *> parseGoalkeepers(QVariantList data);
        QList<GameEvent *> parseShootout(QVariantList data);

        static QMap<uint, League *> mLeaguesMap;

        static const QString SCORES_URL;
        static const QString DETAILS_URL;

    public:
        explicit SIHFDataSource(QObject *parent = 0);
        void update(QString id);
        void setGameId(QString gameId);
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
