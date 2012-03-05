
TARGET = libXtextcursorchanger.so

CC = gcc
CXX = g++
CFLAGS = -O2 -fPIC -shared -Wl,-soname,$(TARGET)
RM = rm -f


HEADERS =
SOURCES = hooks.c


$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@
	cd cursors && ./cursorgen


clean:
	$(RM) $(TARGET)
	$(RM) cursors/*.xcursor
