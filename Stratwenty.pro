QT += core widgets

CONFIG += c++11 qt

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -L/usr/lib/ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lm

SOURCES =  main.cpp \
            src/*

HEADERS = include/*
