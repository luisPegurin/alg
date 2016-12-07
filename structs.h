#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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




void createGrafo(Grafo * g) {
	g->hierarquias = NULL;
}

void createHierarquia(Hierarquia* i) {
	i->topo = NULL;
}

void createInfo(Info* i) {
	i->proximo = NULL;
}

void addInfo(Hierarquia * h, char * nome) {
	Info * i = (Info*) malloc(sizeof(Info));
	if (i == NULL) {
		exit(1);
	}
	createInfo(i);
	strcpy(i->nome,nome);
	if (h->topo == NULL) {
		h->topo = i;
	}
	else {
		Info * aux = h->topo;
		while (aux->proximo != NULL) {
			aux = aux->proximo;		
		}
		aux->proximo = i;
	}
}