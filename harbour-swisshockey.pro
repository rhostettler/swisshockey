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

# QML files & icons
#ui.source = qml/sailfishos
#ui.target = .
#DEPLOYMENTFOLDERS += ui
#app_icons.source = qml/icons
#app_icons.target = .
#DEPLOYMENTFOLDERS += app_icons

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
    src/logger.cpp \
    src/sihfdatasource.cpp \
    src/jsondecoder.cpp

# Add QML files to Qt Creator
OTHER_FILES += qml/sailfishos/harbour-swisshockey.qml \
    qml/sailfishos/cover/CoverPage.qml \
    qml/sailfishos/pages/OverviewPage.qml \
    qml/sailfishos/pages/LeagueSelectionDialog.qml \
    qml/sailfishos/pages/DetailsPage.qml

# Add other files to Qt Creator
OTHER_FILES += rpm/harbour-swisshockey.changes.in \
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
    src/logger.h \
    src/sihfdatasource.h \
    src/jsondecoder.h

#DISTFILES += \
#    qml/sailfishos/pages/OverviewPage.qml \
#    qml/sailfishos/pages/LeagueSelectionDialog.qml \
#    qml/sailfishos/pages/DetailsPage.qml

