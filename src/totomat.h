#ifndef TOTOMAT_H
#define TOTOMAT_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "gamedaydata.h"

#include "json.h"

class Totomat : public QObject {
    Q_OBJECT

    private:
        QNetworkAccessManager *nam;
        QNetworkReply *totomatReply;
        QNetworkReply *statsReply;
        Json *decoder;
        GamedayData *nlaData;
        QTimer *timer;
        QString gameId;

    public:
        explicit Totomat(QObject *parent = 0);
        void setGameId(QString gameId);
        void print(QString message = "");
        void queryTotomat(void);
        void queryStats(void);
        void setData(GamedayData *data);
        qint64 calculateUpdateInterval(QString date);
        Q_INVOKABLE GamedayData* getData();

    signals:

    public slots:
        void parseTotomatResponse();
        void parseStatsResponse();
        void handleNetworkError(QNetworkReply::NetworkError error);
        void update();
};

#endif // TOTOMATREADER_H
