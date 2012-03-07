
TARGET = libXtextcursorchanger.so

CC = gcc
CXX = g++
CFLAGS = -std=c99 -O2 -fPIC -shared -Wl,-soname,$(TARGET)
LIBS = -lX11 -lXcursor

CP = cp
RM = rm -f


HEADERS =
SOURCES = hooks.c layout.c


$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@
	cd cursors && ./cursorgen


install:
	mkdir ~/.cache/libXtextcursorchanger
	cd cursors && cp *.xcursor ~/.cache/libXtextcursorchanger/


clean:
	$(RM) $(TARGET)
	$(RM) cursors/*.xcursor
