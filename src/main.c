/**
 * main.c -- Arquivo main
 *
 * Autores:
 *  Luís Henrique Pegurin 9313325
 *  Mateus Medeiros       9266410
 *  Guilherme Kayo Shida  6878696
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "menu.h"

void imprimiHierarquias (Grafo *g) {
	printf("\n************************************************************************\nHierarquias:");

	for (int i=0; i < g->numHier;i++) {
		Info * aux = g->hierarquias[i]->topo;
		printf("\nDimensão %s(%s)",aux->nome, aux->sigla);
		aux = aux->proximo;
		while (aux != NULL ) {
			printf(" -> %s(%s)",aux->nome, aux->sigla);
			aux = aux->proximo;
		}
	}
	printf("\n************************************************************************\n\n\n");
}


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

		char nome[TAMANHO_MAX_NOME];
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
			char nomeA[TAMANHO_MAX_NOME];
			scanf("%s",nomeA);
			addInfo(h,nomeA);
		}

	}
	calculaSiglas(g);

	imprimiHierarquias(g);

}


void armazenaDados (Grafo *g) {
	char url[10] = "dados";
	int byteOffSet = 0;
	FILE * arq;
	arq = fopen(url, "ab");
	if (arq == NULL) {
			printf("Erro ao abrir arquivo\n");
	}
	else {
		fseek(arq,byteOffSet,SEEK_END);
		for (int i =0; i< g->numHier; i++) {
			Info *aux = g->hierarquias[i]->topo;
			while (aux != NULL) {
				fwrite (aux->nome, 1 ,strlen(aux->nome), arq);
				fwrite ("/",1,1,arq);
				fwrite (aux->sigla,1, strlen(aux->sigla), arq);
				fwrite ("|",1,1,arq);
				aux = aux->proximo;

			}
			fwrite ("*",1,1,arq);
		}
		fwrite ("#",1,1,arq);
	}
	fclose(arq);
}

void leDados(Grafo *g, int rrn) {
	char url[10] = "dados";
	FILE * arq;
	char nome[TAMANHO_MAX_NOME];
	char c;
	char sigla[TAMANHO_MAX_SIGLA];
	arq = fopen(url, "rb");
	if (arq == NULL) {
			printf("Erro ao abrir arquivo\n");
	}
	else {
		int counter=0;
		int pos = 0;
		fseek(arq, 0 ,SEEK_SET);
		// se não for o primeiro registro, deve-se calcular a posição
		if (counter != rrn) {
			c = fgetc(arq);
			while (c != EOF) {
				if (c == '#') {
					counter++;
				}
				if (counter == rrn) {
					c = fgetc(arq);
					break;
				} else {
					c = fgetc(arq);
				}
			}
		}
		// primeiro registro
		else {
			c = fgetc(arq);
		}

		// recupera registro para o grafo
		if (c != EOF) {

			int n=0;
			Hierarquia * vetor[20];

			while (c != '#') {
				Hierarquia* h = (Hierarquia*) malloc(sizeof(Hierarquia));
				if (h == NULL) {
					exit(1);
				}
				createHierarquia(h);
				vetor[n] = h;
				n++;

				while (c != '*') {
					pos = 0;
					while (c != '/' ) {
						nome[pos] = c;
						fread( &c, 1, 1,arq );
						pos++;
					}
					nome[pos] = '\0';
					Info * aux = addInfo(h,nome);

					fread( &c, 1, 1,arq );
					pos = 0;
					while(c != '|') {
						sigla[pos] = c;
						fread( &c, 1, 1,arq );
						pos++;
					}
					sigla[pos] = '\0';
					strcpy(aux->sigla,sigla);
					fread( &c, 1, 1,arq );
				}

				Info * aux2 = h->topo;
				while (aux2 != NULL ) {
					aux2 = aux2->proximo;
				}
				fread( &c, 1, 1,arq );
			}

			g->hierarquias = (Hierarquia**) malloc(sizeof (Hierarquia*) * n);
			g->numHier = n;
			if (g->hierarquias == NULL) {
				exit(1);
			}
			for (int i =0; i<=n; i++) {
				g->hierarquias[i] = vetor[i];
			}
		}
	}

	fclose(arq);
}

int main() {
/*
		const char src[50] = "asd";
	   char dest[50];

	   printf("Before memcpy dest = %s\n", dest);
	   memcpy(dest, src, 20);
	   printf("After memcpy dest = %s\n", dest);


*/
	menu_principal();
	return 0;
}
