#ifndef LIVESCORES_H
#define LIVESCORES_H

#include <QObject>
#include <QString>
#include <QTimer>

#include "qmlapplicationviewer.h"
#include "sihfdatasource.h"
#include "gamedaydata.h"
#include "notifier.h"

class LiveScores : public QObject {
    Q_OBJECT

    private:
        Notifier *notifier;
        GamedayData *nla;
        GameData *current;
        SIHFDataSource *dataSource;
        QmlApplicationViewer *viewer;
        QTimer *timer;

    public:
        explicit LiveScores(QObject *parent = 0);
        ~LiveScores(void);

    signals:

    public slots:
        void updateData();
        void updateView(QString view);
        void toggleFocus(QWidget * old, QWidget * now);
};

#endif // LIVESCORES_H
