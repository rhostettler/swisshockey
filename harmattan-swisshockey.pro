# Add more folders to ship with the application, here
ui.source = qml/harmattan
ui.target = .
DEPLOYMENTFOLDERS = ui

# The icons within the app
app_icons.source = qml/icons
app_icons.target = .
DEPLOYMENTFOLDERS += app_icons

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable
CONFIG += meegotouch

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/json.cpp \
    src/gamedaydata.cpp \
    src/gamedata.cpp \
    src/notifier.cpp \
    src/livescores.cpp \
    src/gameevent.cpp \
    src/datasource.cpp \
    src/sihfdatasource.cpp \
    src/logger.cpp \
    src/config.cpp \
    src/jsondecoder.cpp

# Files used for custom notifications
notification_conf.files = harmattan/notifications/livescores.score.update.conf
notification_conf.path = /usr/share/meegotouch/notifications/eventtypes
INSTALLS += notification_conf

notification_icons.files = icons/icon-m-low-power-mode-livescores.png \
    icons/icon-s-status-notifier-livescores.png
notification_icons.path = /usr/share/themes/blanco/meegotouch/icons
INSTALLS += notification_icons

# Control Panel Applet
cpapplet_desktop.files = harmattan/cpapplet/NLLiveScores.desktop
cpapplet_desktop.path = /usr/share/duicontrolpanel/desktops
INSTALLS += cpapplet_desktop

cpapplet_xml.files = harmattan/cpapplet/NLLiveScores.xml
cpapplet_xml.path = /usr/share/duicontrolpanel/uidescriptions
INSTALLS += cpapplet_xml

# Please do not modify the following two lines. Required for deployment.
include(src/qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \

HEADERS += \
    src/json.h \
    src/gamedaydata.h \
    src/gamedata.h \
    src/notifier.h \
    src/livescores.h \
    src/gameevent.h \
    src/datasource.h \
    src/sihfdatasource.h \
    src/logger.h \
    src/config.h \
    src/jsondecoder.h

# Add Qt dependencies
QT += core network script
PKGCONFIG += gq-gconf

contains(MEEGO_EDITION,harmattan) {
    icon.files = icons/harmattan/swisshockey80.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon
}