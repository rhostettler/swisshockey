#ifndef CONFIG_H
#define CONFIG_H

#include <QVariant>
#include <QString>

// Config class to access GConf in a consistent way
class Config {
    public:
        static Config& getInstance();

        QVariant getValue(QString key, QVariant def);

    private:
        Config() {};

        // Redefine the constructor and '='-operator to avoid spawning (C++ 03)
        Config(Config const&);
        void operator=(Config const&);

};

#endif // CONFIG_H
