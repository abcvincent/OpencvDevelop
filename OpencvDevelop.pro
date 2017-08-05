#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T18:59:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpencvDevelop
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




INCLUDEPATH+=D:/opencv/build/include
INCLUDEPATH+=D:/opencv/build/include/opencv
INCLUDEPATH+=D:/opencv/build/include/opencv2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world320d
else:unix: LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world320

INCLUDEPATH += $$PWD/../../opencv/build/x64/vc14
DEPENDPATH += $$PWD/../../opencv/build/x64/vc14


SOURCES += main.cpp\
        mainwindow.cpp \
    imagelook.cpp \
    mypixitem.cpp \
    myimagelist.cpp \
    codewidget.cpp \
    variatedialog.cpp

HEADERS  += mainwindow.h \
    imagelook.h \
    mypixitem.h \
    myimagelist.h \
    codewidget.h \
    variatedialog.h

FORMS    += mainwindow.ui \
    imagelook.ui \
    myimagelist.ui \
    codewidget.ui

RESOURCES += \
    rc.qrc
