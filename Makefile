CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -g
INCLUDES = -Iinclude
LIBS     = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC      = src/main.c
TARGET   = bin/game.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	$(TARGET)

clean:
	del bin\game.exe