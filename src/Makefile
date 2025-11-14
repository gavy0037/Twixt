# Compiler and compiler flags
CC = gcc
# -Wall: Enable all warnings
# -Wextra: Enable even more warnings
# -g: Generate debugging information
# -I./include: Tell compiler to look for headers in the 'include' directory
CFLAGS = -Wall -Wextra -g -I./include

# Linker flags
LDFLAGS =

# Directories
SRCDIR := ./src
OBJDIR := obj
BINDIR := bin

# Name of the final executable
TARGET := twixt
EXECUTABLE := $(BINDIR)/$(TARGET)

# Find all .c files in the source directory
SOURCES := $(wildcard $(SRCDIR)/*.c)

# Create a list of object files from the source files
# e.g., src/main.c becomes obj/main.o
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# The 'all' target is the default one, executed when you just run 'make'
.PHONY: all
all: $(EXECUTABLE)

# Rule to create the executable by linking object files
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(@D) # Create the bin directory if it doesn't exist
	@printf "LD\t%s\n" $@
	$(CC) $(LDFLAGS) $^ -o $@

# Pattern rule to compile .c source files into .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D) # Create the obj directory if it doesn't exist
	@printf "CC\t%s\n" $<
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the project (remove generated files)
.PHONY: clean
clean:
	@printf "CLEAN\t$(OBJDIR) $(BINDIR)\n"
	@rm -rf $(OBJDIR) $(BINDIR)