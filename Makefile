
TARGET = libtextcursorchanger.so

CC = gcc
CFLAGS = -O2 -fPIC -shared -Wl,-soname,$(TARGET)


HEADERS =
SOURCES = hooks.c


$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -rf $(TARGET)
