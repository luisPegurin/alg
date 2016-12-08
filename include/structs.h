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


// Nó para a lista de ancestrais e filhos dos vertices
typedef struct No No;
typedef struct Vertice Vertice;

struct No {
	No* proximo;
	Vertice * vertice;
};

struct Vertice {
	Info ** infos;
	No * filhos;
	No * ancestrais;
	char* sigla;
};

typedef struct Hierarquia {
	Info * topo;
} Hierarquia;



typedef struct Grafo {
	Hierarquia ** hierarquias;
	int numHier;
	Vertice* verticeTopo;
} Grafo;


void createVertice(Vertice * v);

void createGrafo(Grafo *);

void createHierarquia(Hierarquia *);

void createInfo(Info *);

Info * addInfo(Hierarquia *, char *);

void calculaSiglas(Grafo *);

Vertice * pesquisaVertice( Vertice* v, char* sigla);

void generateGraph(Grafo *g);

void generateFilhos (Vertice * v, Grafo *g) ;



void criaAresta (Vertice * inicio, Vertice* fim);

void geraDot(Grafo * g) ;


void percorreGrafo (Vertice *v, FILE* dot_file) ;

#endif
