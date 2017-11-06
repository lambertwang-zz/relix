FLAGS = -Wall

SRC_LOC = src/
ENGINE_LOC = $(SRC_LOC)engine/
TEST_LOC = test/

INCLUDE = -I$(ENGINE_LOC)

CC = gcc $(FLAGS) $(INCLUDE)

EXECUTABLE = relix

SRC = $(shell find src -name '*.c' ! -name '*relix.c')

SRC_OBJ = $(SRC:.c=.o)

TEST = $(shell find test -name '*.c')

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
	clear

clean_exe:
	rm -rf $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) $(SRC_OBJ) $(TEST_EXE)


