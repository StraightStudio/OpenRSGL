QT += core widgets

CONFIG += c++11 qt

DEFINES += QT_DEPRECATED_WARNINGS

PRECOMPILED_HEADER = depends.h

CONFIG(release){
    LIBS +=-L"/usr/lib/" -L"bin/lib64/" "lib64SDL2-2.0.so.0.8.0" "lib64SDL2_image-2.0.so.0.0.1" "lib64SDL2_mixer-2.0.so.0.0.1" "lib64SDL2_net-2.0.so.0.0.1" "lib64SDL2_ttf-2.0.so.0.14.0" -lm -lpthread
}

SOURCES =  main.cpp \
            src/*

HEADERS = include/*
