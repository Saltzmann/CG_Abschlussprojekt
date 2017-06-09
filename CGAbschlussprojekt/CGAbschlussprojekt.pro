#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T00:46:42
#
#-------------------------------------------------

QT       += core gui opengl

LIBS += -lopengl32

unix: LIBS += -lassimp

win32: LIBS += -L$$PWD/assimp-win-mingw32/code -lassimp
win32: INCLUDEPATH += $$PWD/assimp-win-mingw32/include
win32: DEPENDPATH += $$PWD/assimp-win-mingw32/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CGAbschlussprojekt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    openglcustomwidget.cpp \
    modelloader.cpp \
    collider.cpp \
    collisiondata.cpp \
    rigidbody.cpp \
    physicsengine.cpp \
    bouncingfluidparticle.cpp

HEADERS  += mainwindow.h \
    openglcustomwidget.h \
    modelloader.h \
    collider.h \
    collisiondata.h \
    rigidbody.h \
    physicsengine.h \
    bouncingfluidparticle.h

FORMS    += mainwindow.ui

RESOURCES += \
    models.qrc
