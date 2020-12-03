.PHONY : clean

# GMSQLite Linux Makefile

# run 'make gms1' to build for 32bit

CPPFLAGS = -fPIC -g -Og
TARGET   = libGMSQLite.so

# a handler for 'make gms1'
ifeq (gms1,$(firstword $(MAKECMDGOALS)))
	CPPFLAGS += -m32
	TARGET    = libGMSQLite-32bit.so
endif

LDFLAGS  = -shared
SOURCES  = sqlite3.c gmsqlite.cpp 
HEADERS  = sqlite3.h sqlite3ext.h gmsqlite.hpp 
OBJECTS  = sqlite3.o gmsqlite.o


all: $(TARGET)

gms1: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)


