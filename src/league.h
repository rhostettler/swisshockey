#ifndef LEAGUE_H
#define LEAGUE_H

#include <QObject>
#include <QString>

class League : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString id READ getId)
    Q_PROPERTY(QString abbreviation READ getAbbreviation)
    Q_PROPERTY(QString name READ getName)

    private:
        uint mId;
        QString mAbbreviation;
        QString mName;

    public:
        explicit League(uint id, QString abbreviation, QString name, QObject *parent = 0);
        QString getId(void);
        QString getAbbreviation(void);
        QString getName(void);

    signals:

    public slots:
};

Q_DECLARE_METATYPE(League *)

#endif // LEAGUE_H
