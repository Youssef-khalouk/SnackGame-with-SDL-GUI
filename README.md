# SnackGame-with-SDL-GUI
snack game using sdl as Graphic User Interface.

#MakeFile:

this makefile is for windows using msys2 to install SDL2, and this code is multiplatform.

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

#Image:

![Image](https://github.com/user-attachments/assets/505a38fd-b68a-42e9-86d9-fcb81110bd1f)

