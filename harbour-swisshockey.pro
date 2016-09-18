# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-swisshockey

CONFIG += sailfishapp
PKGCONFIG += mlite5

QT += core network

# Define the platform for platform-specific code
DEFINES += "PLATFORM_SFOS"

SOURCES += \
    src/main.cpp \
    src/livescores.cpp \
    src/config.cpp \
    src/datasource.cpp \
    src/gamedata.cpp \
    src/gamedaydata.cpp \
    src/gameevent.cpp \
    src/json.cpp \
    src/logger.cpp \
    src/sihfdatasource.cpp \

OTHER_FILES += qml/sailfishos/harbour-swisshockey.qml \
    qml/sailfishos/cover/CoverPage.qml \
    qml/sailfishos/pages/FirstPage.qml \
    qml/sailfishos/pages/SecondPage.qml \
    rpm/harbour-swisshockey.changes.in \
    rpm/harbour-swisshockey.spec \
    rpm/harbour-swisshockey.yaml \
    translations/*.ts \
    harbour-swisshockey.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-swisshockey-de.ts

HEADERS += \
    src/livescores.h \
    src/config.h \
    src/datasource.h \
    src/gamedata.h \
    src/gamedaydata.h \
    src/gameevent.h \
    src/json.h \
    src/logger.h \
    src/sihfdatasource.h \

