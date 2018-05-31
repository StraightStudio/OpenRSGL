QT += core

CONFIG += c++11 qt

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -L/usr/lib/ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lm

SOURCES += main.cpp \
    core.cpp \
    scene2d.cpp \
    texloader.cpp \
    actor2d.cpp \
    logger.cpp

HEADERS += \
    core.h \
    scene2d.h \
    texloader.h \
    actor2d.h \
    depends.h \
    logger.h \
    config.h
