CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -g
SRC      = src/main.c

# Unit tests are C++ (doctest), ahead of the C++ migration.
CXX      = c++
CXXFLAGS = -Wall -Wextra -std=c++20 -g
TEST_SRC = tests/test_main.cpp tests/test_game.cpp

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
    TEST_BIN = bin/tests.exe
    RM       = del /q
    RUN      = $(TARGET)
    CLEAN    = del bin\game.exe bin\tests.exe
else ifeq ($(PLATFORM),Darwin)
    BREW_PREFIX ?= $(shell brew --prefix raylib 2>/dev/null || echo /opt/homebrew/opt/raylib)
    INCLUDES = -I$(BREW_PREFIX)/include
    LIBS     = -L$(BREW_PREFIX)/lib -lraylib \
               -framework CoreVideo -framework IOKit -framework Cocoa \
               -framework GLUT -framework OpenGL
    TARGET   = bin/game
    TEST_BIN = bin/tests
    RM       = rm -f
    RUN      = ./$(TARGET)
    CLEAN    = $(RM) $(TARGET) $(TEST_BIN) && $(RM) -r $(TARGET).dSYM $(TEST_BIN).dSYM
else
    INCLUDES = -Iinclude
    LIBS     = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    TARGET   = bin/game
    TEST_BIN = bin/tests
    RM       = rm -f
    RUN      = ./$(TARGET)
    CLEAN    = $(RM) $(TARGET) $(TEST_BIN)
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
# The Model must stay testable without a window, so nothing under src/model/
# may include raylib (phase06 ground rules).
lint:
	@if grep -rnE '^[[:space:]]*#[[:space:]]*include[[:space:]]*[<"](raylib|raymath|rlgl)' src/model; then \
	    echo "src/model must not include raylib"; exit 1; fi
	@command -v cppcheck >/dev/null 2>&1 || { echo "cppcheck not found (install hints are in the Makefile)"; exit 1; }
	cppcheck --std=c99 --enable=warning,style,performance,portability \
	         --error-exitcode=1 --inline-suppr --quiet \
	         --suppress=missingIncludeSystem \
	         --suppress='*:*raylib.h' --suppress='*:*raymath.h' --suppress='*:*rlgl.h' \
	         $(INCLUDES) --library=std $(SRC)

# ── Tests ───────────────────────────────────────────────────────────────────
# doctest unit tests for the game rules. tests/test_game.cpp compiles
# src/main.c in with main() renamed, so no window opens. raylib headers are
# passed as -isystem so warnings inside them do not drown out ours.
TEST_INCLUDES = -Ithird_party $(patsubst -I%,-isystem %,$(INCLUDES))

$(TEST_BIN): $(TEST_SRC) $(SRC) third_party/doctest.h
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) $(TEST_SRC) -o $(TEST_BIN) $(LIBS)

test: $(TEST_BIN)
	./$(TEST_BIN)
