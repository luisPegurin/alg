# Compilador
CP=@gcc

# Variaveis com diretorios
INCLUDE=./include
SRC=./src
OBJ=./obj

# Opcoes de compilacao
FLAGS = -std=c11 -Wall -Werror

all: clean structs menu main
	$(CP) $(OBJ)/structs.o $(OBJ)/menu.o $(OBJ)/main.o -o main

structs:
	$(CP) -c $(SRC)/structs.c $(FLAGS) -I$(INCLUDE) -o $(OBJ)/structs.o

menu:
	$(CP) -c $(SRC)/menu.c $(FLAGS) -I$(INCLUDE) -o $(OBJ)/menu.o

main:
	$(CP) -c $(SRC)/main.c $(FLAGS) -I$(INCLUDE) -o $(OBJ)/main.o

clean:
	rm -f main $(OBJ)/*.o *.dot *.png

run:
	./main
