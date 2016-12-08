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
 * @brief Método que cria grafo
 *
 * @param grafo Ponteiro para um grafo
 * @return Void
 */
void create_grafo(Grafo *grafo);

/**
 * @brief Método que cria uma hierarquia
 *
 * @param hierarquia Ponteiro para uma hierarquia
 * @return Void
 */
void create_hierarquia(Hierarquia *hierarquia);

/**
 * @brief Método que cria uma Info
 *
 * @param info Ponteiro para uma Info
 * @return Void
 */
void create_info(Info *info);

/**
 * @brief Método que cria vértice
 *
 * @param vertice Ponteiro para vertice
 * @return Void
 */
void create_vertice(Vertice *vertice);

/**
 * @brief Método para criar uma aresta
 *
 * @param inicio O vértice de início
 * @param fim    O vértice de fim
 * @return Void
 */
void create_aresta(Vertice *inicio, Vertice *fim);

/**
 * @brief Método para adicionar um novo grafo de derivação
 *
 * @param g Um ponteiro da um tipo Grafo
 * @return Void
 */
void novo_grafo(Grafo* g);

/**
 * @brief Método para adicionar informação de uma dimensão ou atributo
 *
 * @param  hierarquia
 * @param  nome       O nome da dimensão/atributo
 * @return A estrutura com as novas informações
 */
Info *add_info(Hierarquia *hierarquia, char *nome);

/**
 *
 */
void calcula_siglas(Grafo *);

/**
 * @brief	Método para salvar os dados de um novo grafo.
 *
 * @param g O grafo criado com informações de dimensões e atributos.
 * @return Void
 */
void armazena_dados(Grafo *g);

/**
 * @brief Método para ler as informações do arquivo de dados.
 *
 * @param g   O grafo que vai conter as informações do registro.
 * @param rrn A posição do registro no arquivo de dados.
 * @return Void
 */
void leDados(Grafo *g, int rrn);

/**
 *
 */
Vertice *pesquisaVertice(Vertice *, char *);

/**
 *
 */
void generateGraph(Grafo *);

/**
 *
 */
void generateFilhos(Vertice *, Grafo *) ;


/**
 * @brief Método para gerar um arquivo '.dot'
 *
 * @param grafo O grafo que será percorrido para gerar o arquivo '.dot'
 * @return Void
 */
void generate_dot_file(Grafo *grafo);

/**
 * @brief Método que escreve todas as dependências do grafo no arquivo '.dot'
 *
 * @param vertice  O ponto de partida para começar a busca por dependências
 * @param dot_file O arquivo que será escrito as dependências
 * @return Void
 */
void write_all_dependencies(Vertice *vertice, FILE *dot_file) ;

/**
 * @brief Método que gera um arquivo '.png' a partir de um arquivo '.dot'
 *
 * @return Void
 */
void generate_png_from_dot_file();

/**
 * @brief Método para imprimir uma hierarquia (registro)
 *
 * @param grafo O grafo (hierarquia/registro) a ser imprimido
 * @return Void
 */
void imprimir_hierarquias(Grafo *grafo);


#endif
