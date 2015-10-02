#ifndef LIVESCORES_H
#define LIVESCORES_H

#include <QObject>
#include <QString>

#include "qmlapplicationviewer.h"
#include "totomat.h"
#include "gamedaydata.h"
#include "notifier.h"

class LiveScores : public QObject {
    Q_OBJECT

    private:
        Notifier *notifier;
        GamedayData *nla;
        GameData *current;
        Totomat *totomat;
        QmlApplicationViewer *viewer;

    public:
        explicit LiveScores(QObject *parent = 0);
        ~LiveScores(void);

    signals:

    public slots:
        void updateView(QString view);
        void toggleFocus(QWidget * old, QWidget * now);
};

#endif // LIVESCORES_H
