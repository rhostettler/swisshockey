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
        QNetworkReply *totomatReply;  // TODO: Rename
        QNetworkReply *statsReply;    // TODO: Rename
        JsonDecoder *decoder;
        QString gameId;               // TODO: Hmmm?

        QVariantMap parseGameSummary(QVariantList indata);

    public:
        explicit SIHFDataSource(QObject *parent = 0);
        void update(QString id);
        void setGameId(QString gameId);
        void queryScores(void);
        void queryStats(QString gameId);            // TODO: Rename

    signals:

    public slots:
        void parseScoresResponse();
        void parseStatsResponse();
        void handleNetworkError(QNetworkReply::NetworkError error);
        //void update();
};

#endif // SIHFDATASOURCE_H
