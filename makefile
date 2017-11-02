CC = gcc

SRC_LOC = src/

EXECUTABLE = relix

SRC = $(shell find src -name '*.c' ! -name '*relix.c')

SRC_OBJ = $(SRC:.c=.o)

all: clear clean_exe relix 

relix: $(SRC_OBJ)
	$(CC) $(SRC_LOC)$(EXECUTABLE).c -o $(EXECUTABLE) $(SRC_OBJ)

.c.o:
	$(CC) -c $< -o $@

clear:
	clear
	@echo $(SRC)
	@echo $(SRC_OBJ)

clean_exe:
	rm -rf $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) $(SRC_OBJ)


