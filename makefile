FLAGS = -Wall

SRC_LOC = src/
ENGINE_LOC = $(SRC_LOC)engine/
TEST_LOC = test/

INCLUDE = -I$(ENGINE_LOC)

CC = gcc $(FLAGS) $(INCLUDE)


EXECUTABLE = relix

ifeq ($(OS),Windows_NT)
CLEAR = cls
RM = del
# TEST = $(shell find test -name '*.c')

SRC = src/engine/game/game.c \
	src/engine/geometry/geometry.c \
src/engine/input/input.c \
src/engine/list/array.c \
src/engine/list/insert.c \
src/engine/list/iterator.c \
src/engine/list/remove.c \
src/engine/list/tree.c \
src/engine/log/log.c \
src/engine/objects/object.c \
src/engine/objects/objectManager.c \
src/engine/render/render.c \
src/engine/term/color.c \
src/engine/term/screen.c \
src/engine/term/term.c \
src/engine/utility/clock.c \
src/engine/utility/random.c \
src/engine/utility/utility.c \
src/game/map/cellular.c \
src/game/map/map.c \
src/game/map/randomwalk.c \
src/game/map/tunneling.c \
src/game/player/player.c \
src/game/world/world.c
else
CLEAR = clear
RM = rm -rf
SRC = $(shell find src -name '*.c' ! -name '*relix.c')
TEST = $(shell find test -name '*.c')
endif

SRC_OBJ = $(SRC:.c=.o)


TEST_EXE = $(TEST:.c=)

all: clear clean_exe relix 

relix: $(SRC_OBJ)
	$(CC) $(SRC_LOC)$(EXECUTABLE).c -o $(EXECUTABLE) $(SRC_OBJ)

.c.o:
	$(CC) -c $< -o $@

test: clear clean relix $(TEST_EXE)

$(TEST_EXE):
	$(CC) $@.c -o $@ $(SRC_OBJ)

clear:
	$(CLEAR)

clean_exe:
	$(RM) $(EXECUTABLE)

clean:
	$(RM) $(EXECUTABLE) $(SRC_OBJ) $(TEST_EXE)


