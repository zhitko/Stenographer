# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

#symbian:TARGET.UID3 = 0xE52A6A95

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
#symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=
QT += multimedia
QT += network
QT += phonon
# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
# CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    stenographer.cpp \
    wavpcmfile.cpp \
    recthread.cpp \
    googlespeech.cpp \
    audioinfo.cpp \
    downloadandplay.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    stenographer.h \
    stenographer.h \
    wavpcmfile.h \
    recthread.h \
    googlespeech.h \
    audioinfo.h \
    downloadandplay.h

#RESOURCES += \
#    res.qrc

OTHER_FILES += \
    qml/stenographer/Button.qml \
    qml/Button.qml \
    qml/ScrollBar.qml \
    qml/Scrollable.qml \
    qml/Stenographer.qml \
    qml/Log.qml \
    qml/Window.qml \
    qml/Tabs.qml \
    qml/Meter.qml \
    qml/main.qml \
    qml/Spin.qml

#TRANSLATIONS += \
#    tr/messages_ru.ts \
#    tr/messages_eng.ts

RESOURCES += \
    res.qrc
