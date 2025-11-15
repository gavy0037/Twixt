CC = gcc

SRC_DIR := src
OBJ_DIR := obj
BIN := bin

TARGET := twixt

SRC := $(wildcard $(SRC_DIR)/*.c)

.PHONY: all

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $^ -o $@

run: all
	./$(TARGET)

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) $(BIN) twixt.exe