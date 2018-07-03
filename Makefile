TARGET = Stratwenty
ARCH = elf_x86_64

CXX = g++

SRCDIR = src

LIB_PATH=bin/lib64

LD_LIBS = -L/usr/lib64/ -lm -lpthread $(LIB_PATH)/lib64SDL2-2.0.so.0.8.0 $(LIB_PATH)/libSDL2_image-2.0.so.0 $(LIB_PATH)/libSDL2_mixer-2.0.so.0 $(LIB_PATH)/libSDL2_ttf-2.0.so.0 $(LIB_PATH)/libSDL2_net-2.0.so.0
INCLUDE = -I/usr/include/ -I./

CXXFLAGS = -Wall -O2 $(INCLUDE) -fPIC -std=c++17

SRCFILES = $(wildcard $(SRCDIR)/*)
OBJFILES = $(SRCFILES:$(SRCDIR)/%.cpp=%.o)

%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

all-64: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(LD_LIBS) main.cpp -o $(TARGET) $(OBJFILES)
	@echo Compilation complete.

clean:
	@rm -f *.o

distclean: clean
	@rm -f $(TARGET)
