CC = gcc
SRC_DIR := src
TARGET := twixt

SRC := $(wildcard $(SRC_DIR)/*.c)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $^ -o $@

run: clean all
	./$(TARGET)

clean:
	rm -rf	 twixt