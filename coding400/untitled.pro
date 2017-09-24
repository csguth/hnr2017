TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH = /home/csguth/Downloads/dlib-19.7
LIBS = -lX11 -lpthread
QMAKE_LFLAGS = -static-libgcc -static-libstdc++
SOURCES += main.cpp
