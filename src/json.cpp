#include "json.h"

//#include <QtScript/QScriptEngine>
//#include <QtScript/QScriptValueIterator>

Json::Json(QObject *parent) : QObject(parent) {
}

QString Json::encode(const QMap<QString,QVariant> &map) {
    QJSEngine engine;
    engine.evaluate("function toString() { return JSON.stringify(this) }");

    QJSValue toString = engine.globalObject().property("toString");
    QJSValue obj = encodeInner(map, &engine);
    //return toString.call(obj).toString(); // TODO: This is broken!
    return QString();
}

QMap<QString, QVariant> Json::decode(const QString &jsonStr) {
    QJSValue object;
    QJSEngine engine;
    object = engine.evaluate("(" + jsonStr + ")");
    return decodeInner(object);
}

QJSValue Json::encodeInner(const QMap<QString,QVariant> &map, QJSEngine* engine) {
    QJSValue obj = engine->newObject();
    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();
        if (i.value().type() == QVariant::String)
            obj.setProperty(i.key(), i.value().toString());
        else if (i.value().type() == QVariant::Int)
            obj.setProperty(i.key(), i.value().toInt());
        else if (i.value().type() == QVariant::Double)
            obj.setProperty(i.key(), i.value().toDouble());
        else if (i.value().type() == QVariant::List)
            obj.setProperty(i.key(), qScriptValueFromSequence(engine, i.value().toList()));
        else if (i.value().type() == QVariant::Map)
            obj.setProperty(i.key(), encodeInner(i.value().toMap(), engine));
    }
    return obj;
}


QMap<QString, QVariant> Json::decodeInner(QJSValue object) {
    QMap<QString, QVariant> map;
    QJSValueIterator it(object);
    while (it.hasNext()) {
        it.next();
        if (it.value().isArray())
            map.insert(it.name(),QVariant(decodeInnerToList(it.value())));
        else if (it.value().isNumber())
            map.insert(it.name(),QVariant(it.value().toNumber()));
        else if (it.value().isString())
            map.insert(it.name(),QVariant(it.value().toString()));
        else if (it.value().isNull())
            map.insert(it.name(),QVariant());
        else if(it.value().isObject())
            map.insert(it.name(),QVariant(decodeInner(it.value())));
    }
    return map;
}

QList<QVariant> Json::decodeInnerToList(QJSValue arrayValue) {
    QList<QVariant> list;
    QJSValueIterator it(arrayValue);
    while (it.hasNext()) {
        it.next();
        if (it.name() == "length")
            continue;

        if (it.value().isArray())
            list.append(QVariant(decodeInnerToList(it.value())));
        else if (it.value().isNumber())
            list.append(QVariant(it.value().toNumber()));
        else if (it.value().isString())
            list.append(QVariant(it.value().toString()));
        else if (it.value().isNull())
            list.append(QVariant());
        else if(it.value().isObject())
            list.append(QVariant(decodeInner(it.value())));
    }
    return list;
}
