CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -g
SRC      = src/main.c

# ── Platform detection ──────────────────────────────────────────────────────
# Windows (w64devkit): uses the bundled raylib in include/ and lib/.
# macOS: uses raylib from Homebrew (`brew install raylib`).
# Linux: uses a system raylib (`apt install libraylib-dev` or similar).
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
else
    PLATFORM = $(shell uname -s)
endif

ifeq ($(PLATFORM),windows)
    INCLUDES = -Iinclude
    LIBS     = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET   = bin/game.exe
    RM       = del /q
    RUN      = $(TARGET)
    CLEAN    = del bin\game.exe
else ifeq ($(PLATFORM),Darwin)
    BREW_PREFIX ?= $(shell brew --prefix raylib 2>/dev/null || echo /opt/homebrew/opt/raylib)
    INCLUDES = -I$(BREW_PREFIX)/include
    LIBS     = -L$(BREW_PREFIX)/lib -lraylib \
               -framework CoreVideo -framework IOKit -framework Cocoa \
               -framework GLUT -framework OpenGL
    TARGET   = bin/game
    RM       = rm -f
    RUN      = ./$(TARGET)
    CLEAN    = $(RM) $(TARGET) && $(RM) -r $(TARGET).dSYM
else
    INCLUDES = -Iinclude
    LIBS     = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    TARGET   = bin/game
    RM       = rm -f
    RUN      = ./$(TARGET)
    CLEAN    = $(RM) $(TARGET)
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	$(RUN)

clean:
	$(CLEAN)
