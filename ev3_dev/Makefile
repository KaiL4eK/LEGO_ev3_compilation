CC=arm-linux-gnueabi-gcc
LD=$(CC)

INCLUDE_DIRS=include/
LIBRARY_DIRS=lib/

LIBS=ev3c

CFLAGS=-c -Wall -O3 -Iinclude
CFLAGS+=$(addprefix -I,$(INCLUDE_DIRS))

LDFLAGS=$(addprefix -L,$(LIBRARY_DIRS))
LDFLAGS+=$(addprefix -l,$(LIBS))

SOURCES=$(wildcard *.c)
OBJECTS=$(addprefix $(OBJECTS_DIR),$(SOURCES:.c=.o))

BINARY_DIR=bin/
OBJECTS_DIR=obj/

EXECUTABLE=$(BINARY_DIR)main

all: $(BINARY_DIR) $(OBJECTS_DIR) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $@ $< $(LDFLAGS)

$(OBJECTS_DIR)%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BINARY_DIR):
	mkdir -p $(BINARY_DIR)

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

clean:
	rm -rf $(BINARY_DIR) $(OBJECTS_DIR)

install: all
	scp $(EXECUTABLE) robot@10.42.0.3:~/main_prog