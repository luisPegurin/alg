#ifndef STRUCTS_H_
#define STRUCTS_H_

#define tamNome 50
#define tamSigla 10


// representa dimensao ou atributo (hierarquia)
typedef struct Info Info;

typedef struct Info {
	char nome[tamNome];
	char sigla[tamSigla];
	Info * proximo;
} Info;

typedef struct Hierarquia {
	Info * topo;
} Hierarquia;

typedef struct Vertice {
	Info * infos;
} Vertice;

typedef struct Grafo {
	Hierarquia ** hierarquias;
	int numHier;
} Grafo;


void createGrafo(Grafo *);

void createHierarquia(Hierarquia *);

void createInfo(Info *);

Info * addInfo(Hierarquia *, char *);

void calculaSiglas(Grafo *);

#endif
