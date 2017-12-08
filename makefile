FLAGS = -Wall -Wextra -g

SRC_LOC = src/
ENGINE_LOC = $(SRC_LOC)engine/
TEST_LOC = test/
PROJ_NAME = relix

INCLUDE = -I$(ENGINE_LOC)
LINK = -lm -pthread

CC = gcc $(FLAGS) $(INCLUDE)

ifeq ($(OS),Windows_NT)
CLEAR = cls
RM = del /s
TEST = $(shell powershell "gci test -r -i *.c | resolve-path -r")
SRC = $(shell powershell "gci src -r -i *.c | ? Name -ne "relix.c" | resolve-path -r")
EXECUTABLE = $(PROJ_NAME).exe
else
CLEAR = clear
RM = rm -rf
SRC = $(shell find src -name '*.c' ! -name '*relix.c')
TEST = $(shell find test -name '*.c')
EXECUTABLE = $(PROJ_NAME)
endif

SRC_OBJ = $(SRC:.c=.o)


TEST_EXE = $(TEST:.c=)

all: clear clean_exe relix 

relix: $(SRC_OBJ)
	$(CC) $(SRC_LOC)$(PROJ_NAME).c -o $(EXECUTABLE) $(SRC_OBJ) $(LINK)

.c.o:
	$(CC) -c $< -o $@

test: clear clean_test relix $(TEST_EXE)

$(TEST_EXE):
	$(CC) $@.c -o $@ $(SRC_OBJ) $(LINK)

clear:
	$(CLEAR)

clean_exe:
	$(RM) $(EXECUTABLE)

clean_test:
	$(RM) $(TEST_EXE)

clean:
	$(RM) $(EXECUTABLE) $(SRC_OBJ) $(TEST_EXE)

