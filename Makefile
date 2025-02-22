# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -I./bmp

# Source files
SRCS = main.c utils.c bmp/bmp_utils.c

# Object files (placed in /out)
OBJS = $(SRCS:%.c=out/%.o)

# Executable name
TARGET = out/bmp_program

# Default target
all: build

# Ensure the /out directory exists
$(shell mkdir -p out)

# Build the program
build: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files and place them in the /out directory
out/%.o: %.c
	@mkdir -p $(dir $@)  # Create the subdirectory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: clean build
	./$(TARGET)

# Clean up build files
clean:
	rm -rf out

# Phony targets (not actual files)
.PHONY: all build run clean
