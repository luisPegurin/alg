all:
	gcc -o p structs.c structs.h main.c

run:
	./p
clean:
	rm ./p