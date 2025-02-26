# Compiler
CC = gcc

# Compiler flags
# for memory sanitization use: "-g -fsanitize=address"
CFLAGS = -Wall -Wextra -std=c99 -I./bmp

# Source files
SRCS = $(wildcard */*.c)

# Object files (placed in /out)
OBJS = $(SRCS:%.c=out/%.o)

# Executable names
TARGET = out/bmp_program

# Default target
all: build

# Ensure the /out directory exists
$(shell mkdir -p out)

# Build the program with main.c
build: $(OBJS)
	$(CC) $(CFLAGS) main.c -o $(TARGET) $^
	@echo "Program built with main.c. Run with: ./$(TARGET)"

# Build and run the program with test.c
test: $(OBJS)
	$(CC) $(CFLAGS) test.c -o $(TARGET) $^
	@echo "Running test program..."
	./$(TARGET)

# Compile source files into object files and place them in the /out directory
out/%.o: %.c
	@mkdir -p $(dir $@)  # Create the subdirectory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf out

# Phony targets (not actual files)
.PHONY: all build test clean
