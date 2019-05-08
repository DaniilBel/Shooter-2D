#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T21:07:10
#
#-------------------------------------------------

QT       += core gui

win32-msvc*{
LIBS += -luser32
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TargetMotion
TEMPLATE = app


SOURCES += main.cpp\
    boss.cpp \
    hero.cpp \
        widget.cpp \
    customscene.cpp \
    bullet.cpp \
    sprite.cpp

HEADERS  += widget.h \
    boss.h \
    customscene.h \
    bullet.h \
    hero.h \
    sprite.h

FORMS    += widget.ui

RESOURCES += \
    resources/cursor.qrc \
    resources/hero.qrc \
    resources/sprites.qrc
