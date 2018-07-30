TARGET = Stratwenty

CONFIG = c++17

INCLUDEPATH +=  /usr/include/ \
                include/

SOURCES += src/* main.cpp \
    src/scene3d.cpp \
    src/mdlloader.cpp \
    src/texloader.cpp

HEADERS += include/* \
    include/scene3d.h \
    include/mdlloader.h \
    include/texloader.h


DISTFILES += data/shaders/* \
    data/shaders/fbuff.frag \
    data/shaders/fbuff.vert
