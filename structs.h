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
	g->numHier =0;
}

void createHierarquia(Hierarquia* i) {
	i->topo = NULL;
}

void createInfo(Info* i) {
	i->proximo = NULL;
	strcpy(i->nome,"");
	strcpy(i->sigla,"");
}

Info * addInfo(Hierarquia * h, char * nome) {
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
	return i;
}


void calculaSiglas(Grafo *g) {
	int n = g->numHier;
	for (int i=0; i<n;i++) {
		Info *aux = g->hierarquias[i]->topo;
		int Iaux = 0;
		while (aux != NULL) {

			int pos = 0;

			for (int j=0; j<n;j++) {
				Info *aux2 = g->hierarquias[j]->topo;
				int Iaux2 = 0;
				while (aux2 != NULL) {
					if ( (i == j && Iaux2 != Iaux) || i != j ) {
						if ( strcmp(aux2->sigla,"") != 0 ) {
							char s1[tamNome];
							char s2[tamNome];
							strcpy(s1,aux->nome);
							strcpy(s2,aux2->nome);
							s1[pos+1] = '\0';
							s2[pos+1] = '\0';
							while (strcmp(s1,s2) == 0 ) {
								pos++;
								strcpy(s1,aux->nome);
								strcpy(s2,aux2->nome);
								s1[pos+1] = '\0';
								s2[pos+1] = '\0';
							}
						}
					}
					Iaux2++;
					aux2 = aux2->proximo;
				}
			}
			char temp[tamNome];
			strcpy(temp,aux->nome);
			temp[pos+1] = '\0';
			strcpy(aux->sigla,temp);
			Iaux++;
			aux = aux->proximo;
		} 
	}
}
