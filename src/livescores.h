#ifndef LIVESCORES_H
#define LIVESCORES_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QEvent>
#include <QSortFilterProxyModel>

#ifdef PLATFORM_SFOS
// here we'll add the sailfishapp thing
    #include <QQuickView>
    #include <sailfishapp.h>
#else
    #include "qmlapplicationviewer.h"
    #include "notifier.h"
#endif // PLATFORM_SFOS

#include "sihfdatasource.h"
#include "gamedaydata.h"

class LiveScores : public QObject {
    Q_OBJECT

    private:
#ifdef PLATFORM_SFOS
        QQuickView *viewer;
#else
        Notifier *notifier;
        QmlApplicationViewer *viewer;
#endif
        GamedayData *dataStore;
        QSortFilterProxyModel *filter;
        GameData *current;
        SIHFDataSource *dataSource;
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
        void updateLeague(QString leagueId);
};

#endif // LIVESCORES_H
