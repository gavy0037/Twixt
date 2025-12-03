CC = gcc
CFLAGS = -Wall -Wextra -I./include -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
SRC_DIR = src
TARGET = twixt

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Default target
.PHONY: all clean run help

all: $(TARGET)

# Build executable directly without object files
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "✓ Build complete: $(TARGET)"

# Run the program
run: $(TARGET)
	@./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)
	@echo "✓ Clean complete"

# Display help
help:
	@echo "Twixt Game Makefile"
	@echo "===================="
	@echo "make        - Build the project"
	@echo "make run    - Build and run the game"
	@echo "make clean  - Remove executable"
	@echo "make help   - Display this help message"