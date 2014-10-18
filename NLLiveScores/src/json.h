#ifndef JSON_H
#define JSON_H

#include <QObject>
#include <QMap>
#include <QtScript>

/**
  * JSON Parser Class
  *
  * Implementation according to http://stackoverflow.com/questions/4169988/easiest-way-to-parse-json-in-qt-4-7
  *
  */
class Json : public QObject {
    Q_OBJECT

    public:
        explicit Json(QObject *parent = 0);
        QString encode(const QMap<QString, QVariant> &map);
        QMap<QString, QVariant> decode(const QString &jsonStr);

    private:
        QScriptValue encodeInner(const QMap<QString, QVariant> &map, QScriptEngine* engine);
        QMap<QString, QVariant> decodeInner(QScriptValue object);
        QList<QVariant> decodeInnerToList(QScriptValue arrayValue);

    signals:

    public slots:
};

#endif // JSON_H
