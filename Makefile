CC=arm-linux-gnueabi-gcc
CXX=arm-linux-gnueabi-g++	
LD=$(CC)

INCLUDE_DIRS=include/
LIBRARY_DIRS=lib/

BINARY_DIR=bin/
OBJECTS_DIR=obj/

LIBS=ev3dev

CFLAGS=-c -Wall -O3 -Iinclude -std=gnu++0x
CFLAGS+=$(addprefix -I,$(INCLUDE_DIRS))

LDFLAGS=$(addprefix -L,$(LIBRARY_DIRS))
LDFLAGS+=$(addprefix -l,$(LIBS))

SOURCES_CC=$(wildcard *.c)
SOURCES_CXX=$(wildcard *.cpp)

OBJECTS=$(addprefix $(OBJECTS_DIR),$(SOURCES_CXX:.cpp=.o))
OBJECTS+=$(addprefix $(OBJECTS_DIR),$(SOURCES_CC:.c=.o))

EXECUTABLE=$(BINARY_DIR)main

all: $(BINARY_DIR) $(OBJECTS_DIR) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) -o $@ $< $(LDFLAGS)

$(OBJECTS_DIR)%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@

$(OBJECTS_DIR)%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BINARY_DIR):
	mkdir -p $(BINARY_DIR)

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

clean:
	rm -rf $(BINARY_DIR) $(OBJECTS_DIR)

install: all
	cp $(EXECUTABLE) $(HOME)/sshfs
