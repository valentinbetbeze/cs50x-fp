# Compiler flags
FLAGS   = -Wall -fmax-errors=10 -Wextra
# Required object files
OBJ = cli.o utils.o commands.o
# Name of the executable file
EXE     = cli


all: $(EXE)

$(EXE): $(OBJ)
	gcc $(FLAGS) -o $(EXE) $(OBJ) 
	
%.o: %.c
	gcc $(FLAGS) -c -o $@ $<

clear:
	rm -f $(OBJ) $(EXE)

.PHONY: clear