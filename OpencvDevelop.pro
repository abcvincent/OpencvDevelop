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




#INCLUDEPATH+=D:/opencv/build/include
#INCLUDEPATH+=D:/opencv/build/include/opencv
#INCLUDEPATH+=D:/opencv/build/include/opencv2

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world320
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world320d
#else:unix: LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world320

#INCLUDEPATH += $$PWD/../../opencv/build/x64/vc14
#DEPENDPATH += $$PWD/../../opencv/build/x64/vc14





INCLUDEPATH+=C:/opencv/include
INCLUDEPATH+=C:/opencv/include/opencv
INCLUDEPATH+=C:/opencv/include/opencv2

unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_aruco320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_bgsegm320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_bioinspired320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_calib3d320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_ccalib320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_core320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_datasets320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_dnn320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_dpm320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_face320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_features2d320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_flann320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_fuzzy320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_highgui320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_imgcodecs320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_imgproc320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_line_descriptor320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_ml320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_objdetect320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_optflow320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_phase_unwrapping320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_photo320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_plot320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_reg320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_rgbd320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_saliency320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_shape320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_stereo320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_stitching320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_structured_light320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_superres320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_surface_matching320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_text320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_tracking320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_video320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_videoio320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_videostab320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_xfeatures2d320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_ximgproc320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_xobjdetect320d
unix|win32: LIBS += -LC:/opencv/x64/vc14/lib/ -lopencv_xphoto320d

INCLUDEPATH += C:/opencv/x64/vc14
DEPENDPATH += C:/opencv/x64/vc14


SOURCES += main.cpp\
        mainwindow.cpp \
    imagelook.cpp \
    mypixitem.cpp \
    myimagelist.cpp \
    codewidget.cpp \
    variatedialog.cpp \
    histogtamdlg.cpp \
    showhistogtamresult.cpp

HEADERS  += mainwindow.h \
    imagelook.h \
    mypixitem.h \
    myimagelist.h \
    codewidget.h \
    variatedialog.h \
    histogtamdlg.h \
    showhistogtamresult.h

FORMS    += mainwindow.ui \
    imagelook.ui \
    myimagelist.ui \
    codewidget.ui \
    histogtamdlg.ui \
    showhistogtamresult.ui

RESOURCES += \
    rc.qrc
