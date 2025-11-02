# Restaurant Management System Makefile

CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -g -O0 
TARGET = restaurant

# Source files
SOURCES = driver.c copyString.c findMenuItem.c calculateSum.c addMenuItem.c processOrder.c sortMenu.c

# Default target - builds the grade management system
all: $(TARGET)

# Build target
$(TARGET): $(SOURCES) restaurant.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Clean up generated files
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) *.o STUDENT_OUTPUT*.txt
	@echo "Cleanup complete."

# Rebuild everything from scratch
rebuild: clean all

# Show available commands
help:
	@echo "Available commands:"
	@echo "  make           - Build the program"	
	@echo "  make clean     - Remove generated files"
	@echo "  make rebuild   - Clean and build from scratch"

.PHONY: all clean rebuild help