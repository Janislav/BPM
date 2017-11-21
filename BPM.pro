#-------------------------------------------------
#
# Project created by QtCreator 2017-11-14T16:31:51
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BPM
TEMPLATE = app

CONFIG   -= x86_64

LIBS += -framework AudioToolbox
LIBS += -framework Accelerate

ios{
    QMAKE_INFO_PLIST = Info.plist
}


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_IOS_DEPLOYMENT_TARGET=8.0

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        bpm.cpp

HEADERS += \
        bpm.h

FORMS += \
        bpm.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../audio-development/libs/aubio-0.4.5.iosuniversal_framework/release/ -laubio
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../audio-development/libs/aubio-0.4.5.iosuniversal_framework/debug/ -laubio
else:mac: LIBS += -F$$PWD/../../audio-development/libs/aubio-0.4.5.iosuniversal_framework/ -framework aubio
else:unix: LIBS += -L$$PWD/../../audio-development/libs/aubio-0.4.5.iosuniversal_framework/ -laubio

INCLUDEPATH += $$PWD/../../audio-development/libs/aubio-0.4.5.iosuniversal_framework
DEPENDPATH += $$PWD/../../audio-development/libs/aubio-0.4.5.iosuniversal_framework

DISTFILES += \
    Info.plist
