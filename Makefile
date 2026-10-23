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

.PHONY: all run clean docs lint test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	$(RUN)

clean:
	$(CLEAN)

# ── Documentation ───────────────────────────────────────────────────────────
# UML-style struct, call and include diagrams via doxygen + graphviz.
#   macOS:   brew install doxygen graphviz
#   Windows: https://www.doxygen.nl/download.html + https://graphviz.org/download/
docs:
	@command -v doxygen >/dev/null 2>&1 || { echo "doxygen not found (install hints are in the Makefile)"; exit 1; }
	@command -v dot     >/dev/null 2>&1 || echo "warning: graphviz 'dot' not found, diagrams will be skipped"
	doxygen Doxyfile
	@echo "docs written to docs/html/index.html"

# ── Lint ────────────────────────────────────────────────────────────────────
# Static analysis with cppcheck. Fails the build on any finding.
#   macOS:   brew install cppcheck
#   Windows: https://cppcheck.sourceforge.io/
lint:
	@command -v cppcheck >/dev/null 2>&1 || { echo "cppcheck not found (install hints are in the Makefile)"; exit 1; }
	cppcheck --std=c99 --enable=warning,style,performance,portability \
	         --error-exitcode=1 --inline-suppr --quiet \
	         --suppress=missingIncludeSystem \
	         --suppress='*:*raylib.h' --suppress='*:*raymath.h' --suppress='*:*rlgl.h' \
	         $(INCLUDES) --library=std $(SRC)

# ── Tests ───────────────────────────────────────────────────────────────────
# Placeholder until a unit test harness exists. Exits non-zero so CI cannot
# accidentally treat "no tests" as "tests passed".
test:
	@echo "TODO: implement unit tests (see test/ once it exists)"
	@exit 1
