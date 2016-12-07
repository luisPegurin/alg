#include <stdio.h>
#include "structs.h"
#define tamNome 50
#define tamSigla 10



void novoGrafo (Grafo* g) {

	printf("\n**********************************************************************");
	printf("\n(IMPORTANTE)A ordem em que forem inseridas as informações é a ordem da hierarquia\n");
	printf("Digite quantas dimensões terá o grafo: ");
	int n;
	scanf("%d",&n);
	
	g->hierarquias = (Hierarquia**) malloc(sizeof(Hierarquia*) * n);
	g->numHier = n;

	int i;
	for (i=0; i<n; i++) {
		Hierarquia* h = (Hierarquia*) malloc(sizeof(Hierarquia));
		if (h == NULL) {
			exit(1);
		}
		createHierarquia(h);
		g->hierarquias[i] = h;

		char nome[tamNome];
		printf("\n**********************************************************************");
		printf("\nDigite o nome da dimensão %d :", i+1);
		scanf("%s",nome);
		addInfo(h, nome);

		printf("\nDigite o numero de atributos da dimensão %d : ",i+1);
		int m;
		scanf("%d",&m);
		int j;
		for(j=0; j<m;j++) {
			printf("\nDigite o nome do atributo: ");
			char nomeA[tamNome];
			scanf("%s",nomeA);
			addInfo(h,nomeA);
		}

	}

	printf("\n************************************************************************\nHierarquias:");

	for (int i=0; i < g->numHier;i++) {
		Info * aux = g->hierarquias[i]->topo;
		printf("\nDimensão %s",aux->nome);
		aux = aux->proximo;
		while (aux != NULL ) {
			printf(" -> %s",aux->nome);
			aux = aux->proximo;
		}
	}
	printf("\n");
}




int main() {

	Grafo g;
	createGrafo(&g);
	novoGrafo(&g);

	return 0;
}


