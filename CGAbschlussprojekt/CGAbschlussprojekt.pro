#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T00:46:42
#
#-------------------------------------------------

QT       += core gui opengl

# Link against OpenGL for Qt >= 5.5
LIBS += -lopengl32
unix: LIBS += -lassimp

# Windows can't deal with assimp as a system library for some reason
win32: LIBS += -LC:/Users/donpo/Documents/GitHub/CG_Abschlussprojekt/assimp-win-mingw32/code/ -lassimp
win32: INCLUDEPATH += C:/Users/donpo/Documents/GitHub/CG_Abschlussprojekt/assimp-win-mingw32/include
win32: DEPENDPATH += C:/Users/donpo/Documents/GitHub/CG_Abschlussprojekt/assimp-win-mingw32/include

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
    modelloader.cpp \
    customopenglwidget.cpp \
    camera.cpp \
    renderableobject.cpp \
    model.cpp

HEADERS  += mainwindow.h \
    modelloader.h \
    customopenglwidget.h \
    camera.h \
    renderableobject.h \
    model.h

FORMS    += mainwindow.ui

RESOURCES += \
    shader.qrc
