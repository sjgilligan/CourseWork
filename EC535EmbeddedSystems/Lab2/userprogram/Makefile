# arm-linux-gnueabihf-gcc -o ktimer ktimer.c -static -s -Os
CC := arm-linux-gnueabihf-gcc
CFLAGS := -static -s -Os

# Source files
SRC := ktimer.c

# Output executable
TARGET := ktimer

.PHONY: all clean

all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Clean rule
clean:
	rm -f $(TARGET)

