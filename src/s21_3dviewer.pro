QT += core gui opengl openglwidgets

include(./QtGifImage/src/gifimage/qtgifimage.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    back_c/parcer.c \
    back_c/actions.c \
    cpp/glwidget.cpp \
    cpp/main.cpp \
    cpp/mainwindow.cpp

HEADERS += \
    back_c/back_c.h \
    cpp/glwidget.h \
    cpp/mainwindow.h

FORMS += \
    mainwindow.ui\

QMAKE_SPEC_T = $$[QMAKE_SPEC]
contains(QMAKE_SPEC_T,.*win32.*){
    IS_WINDOWS = 1
}
contains(QMAKE_SPEC_T,.*linux.*){
    IS_LINUX = 1
}

TARGET = 3D_Viewer

!isEmpty(IS_WINDOWS): LIBS += -lOpenGL32
!isEmpty(IS_LINUX): LIBS += -lGL

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
