#ifndef LIVESCORES_H
#define LIVESCORES_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QEvent>
#include <QSortFilterProxyModel>

#include "qmlapplicationviewer.h"
#include "sihfdatasource.h"
#include "gamedaydata.h"
#include "notifier.h"

class LiveScores : public QObject {
    Q_OBJECT

    private:
        Notifier *notifier;
        GamedayData *nla;
        QSortFilterProxyModel *filter;
        GameData *current;
        SIHFDataSource *dataSource;
        QmlApplicationViewer *viewer;
        QTimer *timer;
        QString currentId;

    public:
        explicit LiveScores(QObject *parent = 0);
        bool eventFilter(QObject *, QEvent *);
        ~LiveScores(void);

    signals:

    public slots:
        void updateData();
        void updateView(QString view);
        void updateLeague(QString league);
};

#endif // LIVESCORES_H
