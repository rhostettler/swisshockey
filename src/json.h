#ifndef JSON_H
#define JSON_H

#include <QObject>
#include <QMap>

#ifdef PLATFORM_SFOS
    #include <QJSEngine>
    #include <QJSValue>
    #include <QJSValueIterator>


#else
    //#include <QtScript>
    #include <QtScript/QScriptEngine>
    #include <QtScript/QScriptValue>
    #include <QtScript/QScriptValueIterator>

    #define QJSEngine QScriptEngine
    #define QJSValue QScriptValue
    #define QJSValueIterator QScriptValueIterator
#endif



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
        QJSValue encodeInner(const QMap<QString, QVariant> &map, QJSEngine* engine);
        QMap<QString, QVariant> decodeInner(QJSValue object);
        QList<QVariant> decodeInnerToList(QJSValue arrayValue);

    signals:

    public slots:
};

#endif // JSON_H
