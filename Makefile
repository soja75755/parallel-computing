CC         := gcc
LINKER     := $(CC)
CFLAGS     := -Og -Wall -g
LDFLAGS    := -lm
INCLUDES   := -I./include

SOURCES = $(wildcard src/*.c) benchmark.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLES = benchmark

.PHONY: clean

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all:
	make clean;
	make benchmark;

benchmark: $(OBJECTS)
	$(LINKER) $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLES)
