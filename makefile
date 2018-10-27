FLAGS = -Wall -Wextra -g

SRC_LOC = src/
ENGINE_LOC = $(SRC_LOC)engine/
TEST_LOC = test/
PROJ_NAME = relix
ENGINE_NAME = relix_engine.a

INCLUDE = -I$(ENGINE_LOC)
LINK = -lm -pthread

CC = gcc $(FLAGS) $(INCLUDE)

ifeq ($(OS),Windows_NT)
CLEAR = cls
RM = del /F /S
RM_PIPE = 2>nul
TEST = $(shell powershell "gci test -r -i *.c | resolve-path -r")
GAME_SRC = $(shell powershell "gci src\game -r -i *.c | ? Name -ne "relix.c" | resolve-path -r")
ENGINE_SRC = $(shell powershell "gci src\engine -r -i '*.c' | resolve-path -r")
EXECUTABLE = $(PROJ_NAME).exe
else
CLEAR = clear
RM = rm -rf
TEST = $(shell find test -name '*.c')
GAME_SRC = $(shell find src/game -name '*.c' ! -name '*relix.c')
ENGINE_SRC = $(shell find src/engine -name '*.c')
EXECUTABLE = $(PROJ_NAME)
endif

GAME_OBJ = $(GAME_SRC:.c=.o)
ENGINE_OBJ = $(ENGINE_SRC:.c=.o)

TEST_EXE = $(TEST:.c=)

all: clear engine clean_exe relix 

engine: $(ENGINE_OBJ)
	ar rvs $(ENGINE_NAME) $(ENGINE_OBJ)

relix: $(GAME_OBJ)
	$(CC) $(SRC_LOC)$(PROJ_NAME).c -o $(EXECUTABLE) $(GAME_OBJ) $(ENGINE_NAME) $(LINK)

.c.o:
	$(CC) -c $< -o $@

test: clear engine clean_test relix $(TEST_EXE)

$(TEST_EXE):
	$(CC) $@.c -o $@ $(GAME_OBJ) $(ENGINE_NAME) $(LINK)

clear:
	$(CLEAR)

clean_exe:
	$(RM) $(EXECUTABLE) $(RM_PIPE)

clean_test:
	$(RM) $(TEST_EXE) $(RM_PIPE)

clean:
	$(RM) $(EXECUTABLE) $(GAME_OBJ) $(ENGINE_OBJ) $(TEST_EXE) $(RM_PIPE)

