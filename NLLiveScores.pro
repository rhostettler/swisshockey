# Add more folders to ship with the application, here
ui.source = qml
ui.target = .
DEPLOYMENTFOLDERS = ui

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xEF61829D

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable
CONFIG += meegotouch

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/totomat.cpp \
    src/json.cpp \
    src/gamedaydata.cpp \
    src/gamedata.cpp \
    src/notifier.cpp \
    src/livescores.cpp \
    src/gameevent.cpp

# Files used for custom notifications
notification_conf.files = livescores.score.update.conf
notification_conf.path = /usr/share/meegotouch/notifications/eventtypes
INSTALLS += notification_conf

notification_icons.files = icons/icon-m-low-power-mode-livescores.png \
    icons/icon-s-status-notifier-livescores.png
notification_icons.path = /usr/share/themes/blanco/meegotouch/icons
INSTALLS += notification_icons

# Control Panel Applet
cpapplet_desktop.files = cpapplet/NLLiveScores.desktop
cpapplet_desktop.path = /usr/share/duicontrolpanel/desktops
INSTALLS += cpapplet_desktop

cpapplet_xml.files = cpapplet/NLLiveScores.xml
cpapplet_xml.path = /usr/share/duicontrolpanel/uidescriptions
INSTALLS += cpapplet_xml

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
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
    src/totomat.h \
    src/json.h \
    src/gamedaydata.h \
    src/gamedata.h \
    src/notifier.h \
    src/livescores.h \
    src/gameevent.h

# Add Qt dependencies
QT += core network script
PKGCONFIG += gq-gconf
