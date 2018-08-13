TARGET = Stratwenty

CONFIG += c++17 console
CONFIG -= qt

INCLUDEPATH +=  /usr/include/ \
                include/

SOURCES += src/* main.cpp

HEADERS += include/*


DISTFILES += data/shaders/*
