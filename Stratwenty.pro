TARGET = Stratwenty

CONFIG = c++17

INCLUDEPATH +=  /usr/include/ \
                include/

SOURCES += src/* main.cpp \
    ../src/camera.cpp
HEADERS += include/* \
    include/camera.h

DISTFILES += \
    data/shaders/main.frag \
    data/shaders/main.vert
