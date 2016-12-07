#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "structs.h"
#define tamNome 50
#define tamSigla 10


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
	calculaSiglas(g);
	
	imprimiHierarquias(g);
	
}


void armazenaDados (Grafo *g) {

	char url[10] = "dados";
	int byteOffSet = 0;
	char s[50] = "produto";
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

/*
		fseek(arq,byteOffSet,SEEK_SET);
		int n = strlen(s) + sizeof(int);
		fwrite(&n, sizeof(int),1, arq);
		fwrite(s, 1, strlen(s), arq);
//		byteOffSet = strlen(s) + sizeof(int);
//		fseek(arq,byteOffSet,SEEK_SET);
		fwrite(s, 1, strlen(s), arq);
		byteOffSet += 30;
*/
	}
	fclose(arq);

}

void leDados(Grafo *g, int rrn) {
	char url[10] = "dados";
	FILE * arq;
	char nome[tamNome];
	char c;
	char sigla[tamSigla];
	arq = fopen(url, "rb");
	if (arq == NULL) {
			printf("Erro ao abrir arquivo\n");
	}
	else {
		int counter=0;
		int byteOffset = 0;
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

	//printf("%s",a);
}

void menu_principal();
void menu_continuar();
void menu_visualizar_grafo();
void menu_novo_grafo();
void menu_visualizacao_grafica();
void menu_sair();

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

/**
 * @brief Método para mostrar o menu principal
 */
void menu_principal() {
	int menu_opcao;

	printf("*************************************************\n");
	printf("** Bem vindo ao Gerador de Grafos de Derivacao **\n");
	printf("*************************************************\n\n");
	printf("Menu:\n");
	printf("[1] Visualizar grafo\n[2] Novo grafo\n[3] Visualizar graficamente um grafo\n[4] Sair\n\n");
	printf("Opcao: ");

	while (1) {
		fflush(stdin);
		scanf("%d", &menu_opcao);
		if (menu_opcao == 1) {
			menu_visualizar_grafo();
		}
		else if (menu_opcao == 2) {
			menu_novo_grafo();
		}
		else if (menu_opcao == 3) {
			menu_visualizacao_grafica();
		}
		else if (menu_opcao == 4) {
			menu_sair();
		}
	}
}

/**
 * @brief Método para voltar a menu principal ao
 */
void menu_continuar() {
	printf("Aperte [ENTER] para continuar\n");
	while( getchar() != '\n' );
	getchar();
	menu_principal();
}

/**
 * @brief Método para tratar visualização de grafo
 */
void menu_visualizar_grafo() {
	int id_grafo;
	Grafo leitura;

	printf ("\nDigite o número id do grafo: ");
	scanf("%d", &id_grafo);
	
	createGrafo(&leitura);
	leDados(&leitura, id_grafo);
	if (leitura.numHier == 0) {
		printf("\nNão existe esse grafo");
	}
	else {
		imprimiHierarquias(&leitura);
	}

	menu_continuar();
}

/**
 * @brief Método para adicionar um grafo
 */
void menu_novo_grafo() {
	Grafo g;
	createGrafo(&g);

	novoGrafo(&g);

	armazenaDados(&g);

	menu_continuar();
}

/**
 * @brief Método para visualizar graficamente o grafo
 */
void menu_visualizacao_grafica() {
	printf("Opcao 3\n");

	menu_continuar();
}

/**
 * @brief Método para sair do programa
 */
void menu_sair() {
	int i;
	printf("\nSaindo do programa");
	for(i = 0;i < 2;i++) {
    printf(".");
    fflush(stdout);
    sleep(1);
  }
  printf("\n");
	exit(1);
}