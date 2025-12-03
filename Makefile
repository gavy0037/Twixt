CC = gcc
CFLAGS = -Wall -Wextra -I./include -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
SRC_DIR = src
TARGET = twixt

SOURCES = $(wildcard $(SRC_DIR)/*.c)

.PHONY: all clean run help

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	@./$(TARGET)

clean:
	rm -f $(TARGET)