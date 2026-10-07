# CC       = gcc
# CFLAGS   = -Wall -Wextra -std=c99 -g
# INCLUDES = -Iinclude
# LIBS     = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

# SRC      = src/main.c
# TARGET   = bin/game.exe

# all: $(TARGET)

# $(TARGET): $(SRC)
# 	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LIBS)

# run: $(TARGET)
# 	$(TARGET)

# clean:
# 	del bin\game.exe

CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -g
INCLUDES = -Iinclude
LIBS     = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

SRCS     = src/main.c src/character.c
OBJS     = $(SRCS:.c=.o)

TARGET   = bin/game.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LIBS)

src/%.o: src/%.c include/character.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	$(TARGET)

clean:
	@if exist bin\game.exe del /Q bin\game.exe
	@if exist src\main.o del /Q src\main.o
	@if exist src\character.o del /Q src\character.o
