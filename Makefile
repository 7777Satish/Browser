# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Iinclude -g

# Libraries
LIBS = -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

# Directories
SRC_DIR = src
OBJ_DIR = build

# Find all source files recursively
SRCS := $(shell find $(SRC_DIR) -name "*.c")

# Convert src/foo/bar.c -> build/foo/bar.o
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Output executable
TARGET = main

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Run
run: all
	./$(TARGET)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all run clean