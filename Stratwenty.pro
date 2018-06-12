QT += core widgets

CONFIG += c++11 qt

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(release){
    LIBS += -L/usr/lib/ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf -lm -lpthread
}

SOURCES =  main.cpp \
            src/*

HEADERS = include/*
