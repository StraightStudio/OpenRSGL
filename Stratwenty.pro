QT += core widgets

CONFIG += c++11 qt

DEFINES += QT_DEPRECATED_WARNINGS

PRECOMPILED_HEADER = depends.h

unix:CONFIG(release) {
    LIBS += -L/usr/lib64/                       \
            "bin/lib64/libSDL2-2.0.so.0"        \
            "bin/lib64/libSDL2_image-2.0.so.0"  \
            "bin/lib64/libSDL2_mixer-2.0.so.0"  \
            "bin/lib64/libSDL2_net-2.0.so.0"    \
            "bin/lib64/libSDL2_ttf-2.0.so.0"    \
            -lm -lpthread
}

SOURCES =  main.cpp \
            src/*

HEADERS = include/*
