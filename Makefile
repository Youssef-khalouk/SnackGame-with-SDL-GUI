# Compiler and flags
CC = gcc
CFLAGS = -I/mingw64/include/SDL2
LDFLAGS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2

# Source and target
SRC = the_game.c
TARGET = game

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LDFLAGS)

# Clean up build artifacts
clean:
	del /Q $(TARGET).exe 2>nul || true
