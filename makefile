FLAGS = -Wall -Wextra -std=c++11 -g

SRC_LOC = src/
ENGINE_LOC = $(SRC_LOC)engine/
TEST_LOC = test/
PROJ_NAME = relix
ENGINE_NAME = rlxeng.a

INCLUDE = -I$(ENGINE_LOC) -I${SFML_PATH}/include
LINK = -pthread -L${SFML_PATH}/lib -lsfml-graphics -lsfml-window -lsfml-system

CC = g++ $(FLAGS) $(INCLUDE)

ifeq ($(OS),Windows_NT)
CLEAR = cls
RM = del /F /S
RM_PIPE = 2>nul
TEST = $(shell powershell "gci test -r -i *.cpp | resolve-path -r")
GAME_SRC = $(shell powershell "gci src\game -r -i *.cpp | ? Name -ne "relix.c" | resolve-path -r")
ENGINE_SRC = $(shell powershell "gci src\engine -r -i '*.cpp' | resolve-path -r")
EXECUTABLE = $(PROJ_NAME).exe
else
CLEAR = clear
RM = rm -rf
TEST = $(shell find test -name '*.cpp')
GAME_SRC = $(shell find src/game -name '*.cpp' ! -name '*relix.c')
ENGINE_SRC = $(shell find src/engine -name '*.cpp')
EXECUTABLE = $(PROJ_NAME)
endif

GAME_OBJ = $(GAME_SRC:.cpp=.o)
ENGINE_OBJ = $(ENGINE_SRC:.cpp=.o)

TEST_EXE = $(TEST:.cpp=)

all: clear engine clean_exe relix 

engine: $(ENGINE_OBJ)
	ar rvs $(ENGINE_NAME) $(ENGINE_OBJ)

relix: $(GAME_OBJ)
	$(CC) $(SRC_LOC)$(PROJ_NAME).cpp -o $(EXECUTABLE) $(GAME_OBJ) $(ENGINE_NAME) $(LINK)

.cpp.o:
	$(CC) -c $< -o $@

test: clear engine clean_test relix $(TEST_EXE)

$(TEST_EXE):
	$(CC) $@.cpp -o $@ $(GAME_OBJ) $(ENGINE_NAME) $(LINK)

clear:
	$(CLEAR)

clean_exe:
	$(RM) $(EXECUTABLE) $(RM_PIPE)

clean_test:
	$(RM) $(TEST_EXE) $(RM_PIPE)

clean:
	$(RM) $(EXECUTABLE) $(GAME_OBJ) $(ENGINE_OBJ) $(TEST_EXE) $(RM_PIPE)
