#ifndef JSONDECODER_H
#define JSONDECODER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>

#ifdef PLATFORM_SFOS
    #include <QJsonDocument>
    #include <QJsonObject>
#else
    #include "json.h"
#endif

// Wrapper to provide a unique decoder interface to the client
class JsonDecoder : public QObject {
    Q_OBJECT

    public:
        explicit JsonDecoder(QObject *parent = 0);
        QMap<QString, QVariant> decode(QByteArray json);

    signals:

    public slots:

};

#endif // JSONDECODER_H
