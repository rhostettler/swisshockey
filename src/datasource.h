#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QVariantMap>

class DataSource : public QObject {
    Q_OBJECT

    public:
        explicit DataSource(QObject *parent = 0);

        virtual void update() = 0;

    signals:
        void gameSummaryUpdated(QVariantMap data);
        void gameDetailsUpdated();
        void updateError(QString message);

    public slots:
    
};

#endif // DATASOURCE_H
