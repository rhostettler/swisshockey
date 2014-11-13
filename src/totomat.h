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
        QNetworkReply *reply;
        Json *decoder;
        GamedayData *nlaData;
        QTimer *timer;

    public:
        explicit Totomat(QObject *parent = 0);
        void print(QString message = "");
        void query(void);
        void setData(GamedayData *data);
        Q_INVOKABLE GamedayData* getData();

    signals:

    public slots:
        void parseResponse();
        void update();
};

#endif // TOTOMATREADER_H
