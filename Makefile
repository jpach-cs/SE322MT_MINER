CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -g
SRC      = src/main.c

# Unit tests are C++ (doctest), ahead of the C++ migration.
CXX      = c++
CXXFLAGS = -Wall -Wextra -std=c++20 -g
TEST_SRC = $(wildcard tests/*.cpp)

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
    CLEAN    = $(RM) $(TARGET) $(TEST_BIN) && $(RM) -r $(TARGET).dSYM $(TEST_BIN).dSYM bin/coverage coverage
    GCOV     = xcrun llvm-cov gcov
else
    INCLUDES = -Iinclude
    LIBS     = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    TARGET   = bin/game
    TEST_BIN = bin/tests
    RM       = rm -f
    RUN      = ./$(TARGET)
    CLEAN    = $(RM) $(TARGET) $(TEST_BIN) && $(RM) -r bin/coverage coverage
    GCOV     = gcov
endif

.PHONY: all run clean docs lint test coverage coverage-report lint-python test-python

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

# ── Coverage ────────────────────────────────────────────────────────────────
# coverage-report builds the tests with --coverage, runs them, and writes
# coverage/index.html plus coverage/summary.json. It never fails on the number.
#
# coverage runs the report, then the gate in scripts/coverage_gate.py:
#   - line coverage of src/ at or above COVERAGE_MIN passes;
#   - below that, the branch must beat main's coverage (main is measured in a
#     temporary worktree), unless the branch changes nothing in src/ or tests/.
# macOS and Linux only; needs gcovr (brew install gcovr or pip install gcovr).
COVERAGE_FILTER = src/
COVERAGE_MIN    = 90
COVERAGE_BASE   = origin/main
COVERAGE_BIN    = bin/coverage/tests

coverage-report:
	@command -v gcovr >/dev/null 2>&1 || { echo "gcovr not found (brew install gcovr or pip install gcovr)"; exit 1; }
	rm -rf bin/coverage coverage
	mkdir -p bin/coverage coverage
	$(CXX) $(CXXFLAGS) -O0 --coverage $(TEST_INCLUDES) $(TEST_SRC) -o $(COVERAGE_BIN) $(LIBS)
	./$(COVERAGE_BIN)
	gcovr --root . --object-directory bin/coverage --gcov-executable "$(GCOV)" \
	      --filter '$(COVERAGE_FILTER)' --txt --html-details coverage/index.html \
	      --json-summary coverage/summary.json --print-summary

coverage: coverage-report
	python3 scripts/coverage_gate.py --summary coverage/summary.json --minimum $(COVERAGE_MIN) \
	        --base $(COVERAGE_BASE) --make "$(MAKE)" --watch src tests \
	        --make-var CC="$(CC)" --make-var CXX="$(CXX)"

# ── Python tooling ──────────────────────────────────────────────────────────
# The coverage gate is held to the standard it enforces: pylint must score
# 10/10, and its tests must cover at least 90% of it.
#   pip install pylint==4.1.3 pytest==9.1.1 pytest-cov==7.1.0
PYTHON = python3

lint-python:
	PYTHONPATH=scripts:scripts/tests $(PYTHON) -m pylint --fail-under=10 \
	    scripts/coverage_gate.py scripts/tests/*.py

test-python:
	MAKE="$(MAKE)" $(PYTHON) -m pytest scripts/tests -q --cov=coverage_gate \
	    --cov-report=term-missing --cov-fail-under=90
