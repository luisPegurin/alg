all:
	gcc -std=c11 -Wall -o p structs.c structs.h main.c

run:
	./p
clean:
	rm ./p