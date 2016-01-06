#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

class DataSource : public QObject {
    Q_OBJECT

    public:
        explicit DataSource(QObject *parent = 0);

        virtual void update() = 0;

    signals:
        void scoresParsed();
        void gamedetailsParsed();

    public slots:
    
};

#endif // DATASOURCE_H
