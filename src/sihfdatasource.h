#ifndef SIHFDATASOURCE_H
#define SIHFDATASOURCE_H

#include <QString>
#include <QVariantList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "datasource.h"
#include "gamedaydata.h"
#include "json.h"

class SIHFDataSource : public DataSource {
    Q_OBJECT

    private:
        QNetworkAccessManager *nam;
        QNetworkReply *totomatReply;  // TODO: Rename
        QNetworkReply *statsReply;    // TODO: Rename
        Json *decoder;
        GamedayData *nlaData;         // TODO: Remove, will communicate through signals
        QString gameId;               // TODO: Hmmm?

        QVariantMap parseGameSummary(QVariantList indata);

    public:
        explicit SIHFDataSource(QObject *parent = 0);
        void update();
        void setGameId(QString gameId);
        void queryScores(void);
        void queryStats(void);            // TODO: Rename
        void setData(GamedayData *data);  // TODO: Hmmm?
        qint64 calculateUpdateInterval(QString date);  // TODO: Remove?
        Q_INVOKABLE GamedayData* getData();  // TODO: Hmmm?

    signals:

    public slots:
        void parseScoresResponse();
        void parseStatsResponse();
        void handleNetworkError(QNetworkReply::NetworkError error);
        //void update();
};

#endif // SIHFDATASOURCE_H
