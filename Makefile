CC = gcc
DATE := $(shell git log -n 1 --format="%ad" --date=format-local:'%Y%m%d/%H:%M:%S')
CFLAGS = `pkg-config --cflags gtk+-3.0` -Dmver=\"$(DATE)\"  -I/usr/include/X11/extensions/
LIBS = `pkg-config --libs gtk+-3.0`  -I/usr/include/X11/extensions/Xrandr.h

# File names
SRC = sgrandr.c sgrandr-cr.c
OBJ = $(SRC:.c=.o)
EXE = sgrandr sgrandr-cr

# Build executable files
all: $(EXE)

sgrandr: sgrandr.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

sgrandr-cr: sgrandr-cr.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Debug step
debug:
	$(CC) $(CFLAGS) -g $(SRC) -o debug $(LIBS)

# Test step
test:
	./sgrandr
	./sgrandr-cr

# Clean object files and executables
clean:
	rm -f $(OBJ) $(EXE) debug
