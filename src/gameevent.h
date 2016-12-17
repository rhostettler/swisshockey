#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>

class GameEvent : public QObject {
    Q_OBJECT

    public:
        enum EventType {
            GOAL = 1,
            PENALTY,
            PENALTY_SHOT,
            GOALKEEPER_OUT,
            GOALKEEPER_IN
        };

        // TODO: I should find better names for these I think.
        enum PlayerRole {
            SCORER = 1,
            FIRST_ASSIST,
            SECOND_ASSIST,
            PENALIZED,
            GOALKEEPER
        };

    private:
        // Stores the event type
        int type;

        // Stores the event time in seconds
        float time;

        // List of players involved in this event
        QMap<int, quint32> players;

        // Stores the ID of the team this event belongs to
        qlonglong team;

        // For events of EVENT_TYPE::GOAL only: Stores the score
        QString score;
        QString scoreType;

        // For events of EVENT_TYPE::PENALTY only: Stores the penalty ID and type (minutes)
        int penaltyId;
        QString penaltyType;

        //
        bool penaltyShot;

        // List of human-readable penalty texts
        static QList<QString> penaltyTexts;

    public:
        explicit GameEvent(int type);
        int getType(void);
        void setTime(QString time);
        float getTime(void) const;
        void setTeam(qlonglong team);
        qlonglong getTeam(void);
        void addPlayer(int role, quint32 playerId);
        void setScore(QString score, QString type);
        void setPenalty(int id, QString type);
        int getPenalty(void);
        void setPenaltyShot(bool scored);

        // TODO: These are convenience methods and are to disappear/change in
        // future versions
        quint32 getPlayer(void) const;
        quint32 getPlayer(int role) const;
        QString getAdditionalInfo(void) const;
        QString getEvent(void) const;
        QString getEventSubtext(void) const;

        static bool greaterThan(const GameEvent *e1, const GameEvent *e2);
        static QString getPenaltyText(int id);
};

#endif // GAMEEVENT_H
