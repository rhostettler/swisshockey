#include "jsondecoder.h"

JsonDecoder::JsonDecoder(QObject *parent) : QObject(parent) {
}

#ifdef PLATFORM_SFOS
QMap<QString, QVariant> JsonDecoder::decode(QByteArray json) {
    QMap<QString, QVariant> map;
    QJsonDocument document = QJsonDocument::fromJson(json);

    if(document.isObject()) {
        map = document.object().toVariantMap();
    } else if(document.isArray()) {
        //map = document.array().to
    }

    return map;
}

#else
QMap<QString, QVariant> JsonDecoder::decode(QByteArray json) {
    Json decoder;
    QString jsonStr = json.data();

    return decoder.decode(jsonStr);
}
#endif
