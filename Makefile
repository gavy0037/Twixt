CC = gcc
CFLAGS = -Wall -Wextra -I./include
TAR = twixt#target

SOURCES = $(wildcard src/*.c)

.PHONY: all clean run

all: $(TAR)

$(TAR): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TAR)
	@./$(TAR)

clean:
	rm -f $(TAR)