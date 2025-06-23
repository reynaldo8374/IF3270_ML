# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -g

# Source files
SRCS = main.c interface.c dokter_handler.c jadwal_generator.c jadwal_io.c

# Header files
HEADERS = definitions.h interface.h dokter_handler.h jadwal_generator.h jadwal_io.h

# OS-specific settings
ifeq ($(OS),Windows_NT)
    TARGET = jadwal_app.exe
    RM = del /Q
    RUN_CMD = .\\$(TARGET)
else
    TARGET = jadwal_app
    RM = rm -f
    RUN_CMD = ./$(TARGET)
endif

OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking target
$(TARGET): $(OBJS)
	@echo ">> Linking objects -> $@"
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo ">> Build complete: $(TARGET)"

# Compilation target
%.o: %.c $(HEADERS)
	@echo ">> Compiling $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Run target
run: all
	@echo ">> Running application..."
	@echo "--------------------------"
	$(RUN_CMD)
	@echo "--------------------------"
	@echo ">> Application finished."

# Clean target
clean:
	@echo ">> Cleaning build files..."
	-$(RM) $(OBJS) $(TARGET)
	@echo ">> Done."

.PHONY: all clean run
