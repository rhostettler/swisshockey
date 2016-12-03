#ifdef PLATFORM_SFOS
    #include <mlite5/mgconfitem.h>
#else
    #include <gconfitem.h>
#endif

#include "config.h"

Config& Config::getInstance() {
    // Create an instance upon the first call that is guaranteed to be destroyed
    // upon deletion of the object
    static Config instance;
    return instance;
}

QVariant Config::getValue(QString key, QVariant def) {
    QString gconfPath = "/apps/NLLiveScores/settings/";
    QString gconfKey = gconfPath.append(key);
#ifdef PLATFORM_SFOS
    MGConfItem item(gconfKey);
#else
    GConfItem item(gconfKey);
#endif
    return item.value(def);
}
