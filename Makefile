CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lraylib -lm

SRC = src/main.c src/world.c src/physics.c src/render.c src/particle.c
OBJ = $(SRC:.c=.o)
TARGET = collisions

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
run:
	./collisions
