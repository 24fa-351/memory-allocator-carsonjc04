# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# File names
TARGET = test_memory_manager
SOURCES = test-memory-manager.c memory-allocator.c
HEADERS = heap-memory-manager.h

# Default target to build system malloc version
system: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET)_system $(SOURCES)

# Target to build custom malloc version
custom: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -DTEST_CUSTOM_MALLOC -o $(TARGET)_custom $(SOURCES)

# Clean up generated files
clean:
	rm -f $(TARGET)_system $(TARGET)_custom

