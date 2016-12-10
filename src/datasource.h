#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QVariantMap>

#include "gameevent.h"

class DataSource : public QObject {
    Q_OBJECT

    public:
        explicit DataSource(QObject *parent = 0);

        virtual void update(QString id) = 0;

    signals:
        void gameSummaryUpdated(QVariantMap data);
        void gameDetailsUpdated(QList<GameEvent *> events, QVariantList players);
        void updateError(QString message);
        void updateStarted();
        void updateFinished();

    public slots:
    
};

#endif // DATASOURCE_H
