#-------------------------------------------------
#
# Project created by QtCreator 2020-02-23T20:05:04
#
#-------------------------------------------------

QT       += core gui
CONFIG += C++11
QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = videoParser
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    videoparser.cpp \
    codec/h264_sei.cpp \
    codec/h264_stream.cpp \
    codec/h265_stream.cpp \
    codec/NalParse.cpp \
    codec/h265_sei.cpp \
    commands.cpp \
    chunks.cpp \
    qhexedit.cpp \
    codec/H264Decode.cpp

HEADERS += \
        mainwindow.h \
    videoparser.h \
    codec/h264_stream.h \
    codec/h265_stream.h \
    codec/bs.h \
    codec/h264_sei.h \
    codec/h264_stream.h \
    codec/h265_stream.h \
    codec/bs.h \
    codec/h264_sei.h \
    codec/h264_stream.h \
    codec/h265_stream.h \
    codec/NalParse.h \
    qhexedit.h \
    chunks.h \
    commands.h \
    codec/H264Decode.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += $$PWD/codec/
win32{

INCLUDEPATH += $$PWD/ffmpeg/include

LIBS += -L$$PWD\ffmpeg\lib -lavformat -lavcodec -lavutil  -liconv -lswresample   -lgcc -lmingwex  -lswscale
}
unix{

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil  -liconv -lswresample  -lswscale
}
