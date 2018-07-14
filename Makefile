PDIR=../Stratwenty/

VERSION=$(shell date +%d%m%Y) prerelease

TARGET=Stratwenty
ARCH=elf_x86_64

DEBUG=-g

CXX=g++

SRCDIR=src

LIB_PATH=bin/lib64

LD_LIBS=-L/usr/lib64/ -lm -lpthread $(LIB_PATH)/lib64SDL2-2.0.so.0.8.0 $(LIB_PATH)/libSDL2_image-2.0.so.0 $(LIB_PATH)/libSDL2_mixer-2.0.so.0 $(LIB_PATH)/libSDL2_ttf-2.0.so.0 $(LIB_PATH)/libSDL2_net-2.0.so.0
INCLUDE=-I/usr/include/ -I./

CXXFLAGS=-Wall -O2 -fstrict-aliasing $(INCLUDE) $(DEBUG) -fPIC -std=c++17

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
	@tar -cvpf Stratwenty_release.tar.gz $(PDIR)Stratwenty $(PDIR)start.sh $(PDIR)bin/ $(PDIR)data/config.json $(PDIR)data/audio/music/*.ogg $(PDIR)data/audio/sounds/*.wav $(PDIR)data/fonts/ $(PDIR)data/res/ $(PDIR)data/scenes/ $(PDIR)data/models/ $(PDIR)data/img/*.png $(PDIR)data/img/*.json

clean:
	@rm -f *.o

distclean: clean
	@rm -f $(TARGET)
