DATE := $(shell git log -n 1 --format="%ad" --date=format-local:'%Y%m%d/%H:%M:%S')

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Dmver=\"$(DATE)\" -lX11 -lXrandr
LIBS = `pkg-config --libs gtk+-3.0`

# File names
SRC = sgrandr.c
OBJ = $(SRC:.c=.o)
EXE = sgrandr

# Build executable files
all: $(EXE)

sgrandr: sgrandr.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Debug step
debug:
	$(CC) $(CFLAGS) -g $(SRC) -o debug $(LIBS)

# Test step
test:
	./sgrandr

# Clean object files and executables
clean:
	rm -f $(OBJ) $(EXE) debug
