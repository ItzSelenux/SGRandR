# Compiler and linker flags
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtk4`
LDFLAGS = `pkg-config --libs gtk+-3.0` `pkg-config --libs gtk4`

# Source files
SRCS = selerandr.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = selerandr

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

