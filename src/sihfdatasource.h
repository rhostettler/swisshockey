#ifndef SIHFDATASOURCE_H
#define SIHFDATASOURCE_H

#include <QString>
#include <QVariantList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "datasource.h"
#include "gamedaydata.h"
#include "jsondecoder.h"

class SIHFDataSource : public DataSource {
    Q_OBJECT

    private:
        QNetworkAccessManager *nam;
        QNetworkReply *summariesReply;
        QNetworkReply *detailsReply;
        JsonDecoder *decoder;
        QString gameId; // TODO: Should this be here?

        // Private helper functions
        QVariantMap parseSummaries(QVariantList indata);
        QList<GameEvent *> parseGoals(QVariantList data);
        QList<GameEvent *> parsePenalties(QVariantList data);
        QList<GameEvent *> parseGoalkeepers(QVariantList data);
        QList<GameEvent *> parseShootout(QVariantList data);

        static QMap<QString, QString> leagues;

        static const QString SCORES_URL;
        static const QString DETAILS_URL;

    public:
        explicit SIHFDataSource(QObject *parent = 0);
        void update(QString id);
        void setGameId(QString gameId);
        void queryScores(void);
        void queryDetails(QString gameId);

        static QString getLeagueId(QString name);
        static const QMap<QString, QString> initLeagueList(void);

    signals:

    public slots:
        void parseSummariesResponse();
        void parseDetailsResponse();
        void handleNetworkError(QNetworkReply::NetworkError error);
        //void update();
};

#endif // SIHFDATASOURCE_H
