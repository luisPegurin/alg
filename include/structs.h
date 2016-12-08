/**
 * structs.h -- Definição das estruturas e métodos utilizados no programa.
 *
 * Autores:
 *  Luís Henrique Pegurin 9313325
 *  Mateus Medeiros       9266410
 *  Guilherme Kayo Shida  6878696
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

/**
 * TAMANHO_MAX_NOME é o tamanho máximo para o valor do nome na estrutura Info
 */
#define TAMANHO_MAX_NOME 50

/**
 * TAMANHO_MAX_SIGLA é o tamanho máximo para o valor da sigla na estrutura Info
 */
#define TAMANHO_MAX_SIGLA 10


// representa dimensao ou atributo (hierarquia)
typedef struct Info Info;

typedef struct Info {
	char nome[TAMANHO_MAX_NOME];
	char sigla[TAMANHO_MAX_SIGLA];
	Info *proximo;
} Info;


// Nó para a lista de ancestrais e filhos dos vertices
typedef struct No No;
typedef struct Vertice Vertice;

struct No {
	No *proximo;
	Vertice *vertice;
};

struct Vertice {
	Info **infos;
	No *filhos;
	No *ancestrais;
	char *sigla;
};

typedef struct Hierarquia {
	Info *topo;
} Hierarquia;

typedef struct Grafo {
	Hierarquia **hierarquias;
	int numHier;
	Vertice *verticeTopo;
} Grafo;

/**
 * @brief Método para criar vértice
 */
void createVertice(Vertice *);

/**
 * @brief Método para criar grafo
 */
void createGrafo(Grafo *);

void createHierarquia(Hierarquia *);

void createInfo(Info *);

Info *addInfo(Hierarquia *, char *);

void calculaSiglas(Grafo *);

Vertice *pesquisaVertice(Vertice *, char *);

void generateGraph(Grafo *);

void generateFilhos(Vertice *, Grafo *) ;

void createAresta(Vertice *, Vertice *);

/**
 * @brief Gera um arquivo '.dot'
 *
 * @param grafo O grafo que será percorrido para gerar o arquivo '.dot'
 * @return Void
 */
void generate_dot_file(Grafo *grafo);

/**
 * @brief Gera um arquivo '.png' a partir de um arquivo '.dot'
 *
 * @return Void
 */
void generate_png_from_dot_file();

void percorreGrafo(Vertice *, FILE *) ;

#endif
