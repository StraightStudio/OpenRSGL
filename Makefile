VERSION=$(shell date +%m%Y) prerelease

TARGET=OpenRSGL
ARCH=elf_x86_64

DEBUG=-g

CXX=g++

SRCDIR=src

LIB_PATH=bin/lib64

LD_LIBS=-L/usr/lib64/				\
		-L$(LIB_PATH)			\
		-lGLEW -lGLU -lm -lGL -lm 	\
		-lpthread -pthread -ldl -ldrm	\
		-lXdamage -lXfixes -lX11-xcb 	\
		-lxcb-glx -lxcb-dri2 -lXxf86vm 	\
		-lXext -lX11 -lpthread -lxcb	\
		 -lXau -lXdmcp 			\
		-lSDL2				\
		-lSDL2_image			\
		-lSDL2_mixer			\
		-lSDL2_net			\
		-lsteam_api			\
		-ldl				\
		-lGL				\
		-lGLU				\
		-lGLEW				\
		-lpng				\
		-lopenal

INCLUDE=-I/usr/include/ -Iinclude/

NOWARNS=-Wno-parentheses -Wno-write-strings -Wno-narrowing
CXXFLAGS= $(NOWARNS) -O2 $(INCLUDE) $(DEBUG) -std=c++11

SRCFILES=$(wildcard $(SRCDIR)/*)
OBJFILES=$(SRCFILES:$(SRCDIR)/%.cpp=%.o)

%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: $(OBJFILES)
	@echo Building $(VERSION) version
	@sed -i 's/#define DW_VERSION.*/#define DW_VERSION "$(VERSION)"/g' include/config.h
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(OBJFILES) $(LD_LIBS)
	@echo Compilation complete.

shipment:
	@tar -cvpf $(TARGET)_release.tar.gz ../$(TARGET)/$(TARGET) ../$(TARGET)/$(TARGET).exe ../$(TARGET)/start.sh ../$(TARGET)/bin/ ../$(TARGET)/data/datapkg ../$(TARGET)/data/config.json ../$(TARGET)/steam_appid.txt ../$(TARGET)/start.bat

clean:
	@rm -f *.o

distclean: clean
	@rm -f $(TARGET)
