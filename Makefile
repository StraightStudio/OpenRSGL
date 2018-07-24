VERSION=$(shell date +%m%Y) prerelease

TARGET=Stratwenty
ARCH=elf_x86_64

DEBUG=-g

CXX=g++

SRCDIR=src

LIB_PATH=bin/lib64

LD_LIBS=-L/usr/lib64/				\
		-L$(LIB_PATH)			\
		-lm -lpthread			\
		-lSDL2				\
		-lSDL2_image			\
		-lSDL2_mixer			\
		-lSDL2_ttf			\
		-lSDL2_net			\
		-lsteam_api			\
		-ldl				\
		-lGL				\
		-lGLU				\
		-lGLEW				\
		-L$(LIB_PATH)/libsteam.a	\
		-L$(LIB_PATH)/steamclient.a

INCLUDE=-I/usr/include/ -Iinclude/

CXXFLAGS=-Wno-parentheses -O2 $(INCLUDE) $(DEBUG) -std=c++17

SRCFILES=$(wildcard $(SRCDIR)/*)
OBJFILES=$(SRCFILES:$(SRCDIR)/%.cpp=%.o)

%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: $(OBJFILES)
	@echo Building $(VERSION) version
	@sed -i 's/#define DW_VERSION.*/#define DW_VERSION "$(VERSION)"/g' include/config.h
	$(CXX) $(CXXFLAGS) $(LD_LIBS) main.cpp -o $(TARGET) $(OBJFILES)
	@echo Compilation complete.

shipment:
	@tar -cvpf Stratwenty_release.tar.gz ../$(TARGET)/$(TARGET) ../$(TARGET)/$(TARGET).exe ../$(TARGET)/start.sh ../$(TARGET)/bin/ ../$(TARGET)/data/datapkg ../$(TARGET)/data/config.json ../$(TARGET)/steam_appid.txt ../$(TARGET)/start.bat

clean:
	@rm -f *.o

distclean: clean
	@rm -f $(TARGET)
