CC = gcc

CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lglfw -lGL -lm

SRC = \
src/main.c \
src/world.c \
src/particle.c \
src/grid.c \
src/physicsCommon.c \
src/physicsNaive.c \
src/physicsGrid.c \
src/render.c \
glad.c

OBJ = $(SRC:.c=.o)
TARGET = collisions

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

run:
	./$(TARGET)
