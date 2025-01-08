CC         := gcc
LINKER     := $(CC)
CFLAGS     := -Og -Wall -g
LDFLAGS    := -lm
INCLUDES   := -I./include

SOURCES = $(wildcard src/*.c) benchmark.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLES = benchmark

.PHONY: clean

src/utils.o: src/utils.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

src/simd.o: src/simd.c
	$(CC) $(CFLAGS) -msse $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -mno-sse $(INCLUDES) -c $< -o $@

all:
	make clean;
	make benchmark;

benchmark: $(OBJECTS)
	$(LINKER) $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLES)
