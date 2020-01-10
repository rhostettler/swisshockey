# The name of your application
TARGET = harbour-swisshockey

# Disable qml b/c we don't want to ship the whole qml directory (i.e. exclude
# the harmattan files)
#CONFIG += sailfishapp_no_deploy_qml
CONFIG += sailfishapp
QT += core network dbus
PKGCONFIG += mlite5 nemonotifications-qt5


# QML files & icons
qml.files = qml
qml.path = /usr/share/$${TARGET}
app_icons.files = qml/icons
app_icons.path = /usr/share/$${TARGET}
INSTALLS += qml app_icons

# Define the platform for platform-specific code
DEFINES += "PLATFORM_SFOS"

#DEFINES += APP_VERSION=\$${VERSION}\\
DEFINES += "APP_NAME='\"Swiss Ice Hockey\"'"

SOURCES += \
    src/main.cpp \
    src/livescores.cpp \
    src/config.cpp \
    src/datasource.cpp \
    src/gamedata.cpp \
    src/gamedaydata.cpp \
    src/gameevent.cpp \
    src/logger.cpp \
    src/sihfdatasource.cpp \
    src/jsondecoder.cpp \
    src/league.cpp \
    src/player.cpp \
    src/notifier.cpp

# Add QML files to Qt Creator
OTHER_FILES += qml/harbour-swisshockey.qml \
    qml/cover/CoverPage.qml \
    qml/pages/OverviewPage.qml \
    qml/pages/GameOverviewDelegate.qml \
    qml/pages/LeagueSelectionDialog.qml \
    qml/pages/DetailsPage.qml \
    qml/pages/GameDetailsDelegate.qml \
    qml/pages/GameDetailsHeader.qml \
    qml/pages/utils.js

# Add other files to Qt Creator
OTHER_FILES += rpm/harbour-swisshockey.changes \
    rpm/harbour-swisshockey.spec \
    rpm/harbour-swisshockey.yaml \
    translations/*.ts \
    harbour-swisshockey.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
#CONFIG += sailfishapp_i18n
#TRANSLATIONS += translations/harbour-swisshockey-de.ts

HEADERS += \
    src/livescores.h \
    src/config.h \
    src/datasource.h \
    src/gamedata.h \
    src/gamedaydata.h \
    src/gameevent.h \
    src/logger.h \
    src/sihfdatasource.h \
    src/jsondecoder.h \
    src/league.h \
    src/player.h \
    src/notifier.h
