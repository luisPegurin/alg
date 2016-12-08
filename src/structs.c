/**
 * structs.c -- Implementação dos métodos para resolver os problemas propostos.
 *
 * Autores:
 *  Luís Henrique Pegurin 9313325
 *  Mateus Medeiros       9266410
 *  Guilherme Kayo Shida  6878696
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "structs.h"

/**
 * Inicialização de uma estrutura Grafo
 */
void create_grafo(Grafo *grafo) {
  grafo->hierarquias = NULL;
  grafo->numHier = 0;
}


/**
 * Inicialização de uma estrutura Hierarquia
 */
void create_hierarquia(Hierarquia* hierarquia) {
  hierarquia->topo = NULL;
}


/**
 * Inicialização de uma estrutura Info
 */
void create_info(Info* info) {
  info->proximo = NULL;
  strcpy(info->nome, "");
  strcpy(info->sigla, "");
}


/**
 * Inicialização de uma estrutura Vertice
 */
void create_vertice(Vertice *vertice) {
  vertice->infos = NULL;
  vertice->filhos = NULL;
  vertice->ancestrais = NULL;
}


/**
 * Cria-se uma aresta entre dois vértices passados como parâmetros. Um vértice
 * representa o início e o outro o fim.
 */
void create_aresta(Vertice *inicio, Vertice *fim) {
  No *filho = (No*) malloc(sizeof(No));
  No *ancestral = (No*) malloc(sizeof(No));

  if (filho == NULL || ancestral == NULL) {
    exit(1);
  }

  filho->vertice = fim;
  ancestral->vertice = inicio;

  filho->proximo = inicio->filhos;
  inicio->filhos = filho;

  ancestral->proximo = fim->ancestrais;
  fim->ancestrais = ancestral;
}


/**
 * Cria-se um novo grafo de derivação pedindo ao usuário o número de dimensões,
 * seu nome, a quantidade de atributos e seus respectivos nome.
 * Um detalhe importante é o fato de que a hierarquia é determinada pela ordem
 * de inserção das informações.
 * Ao terminar de inserir as informações, calcula-se as siglas de cada dimensão
 * /atributo e imprime a hierarquia na tela.
 */
void new_grafo(Grafo* g) {
  printf("\n**********************************************************************");
  printf("\n(IMPORTANTE) A ordem em que forem inseridas as informações é a ordem da hierarquia\n");
  printf("Digite quantas dimensões terá o grafo: ");
  int i;
  int n;

  scanf("%d",&n);

  g->hierarquias = (Hierarquia**) malloc(sizeof(Hierarquia*) * n);
  g->numHier = n;

  for (i = 0; i < n; i++) {
    Hierarquia* h = (Hierarquia*) malloc(sizeof(Hierarquia));
    if (h == NULL) {
      exit(1);
    }
    create_hierarquia(h);
    g->hierarquias[i] = h;

    char nome[TAMANHO_MAX_NOME];
    printf("\n**********************************************************************");
    printf("\nDigite o nome da dimensão %d :", i+1);
    scanf("%s",nome);
    add_info(h, nome);

    printf("\nDigite o numero de atributos da dimensão %d : ",i+1);
    int m;
    scanf("%d",&m);
    int j;
    for(j=0; j<m;j++) {
      printf("\nDigite o nome do atributo: ");
      char nomeA[TAMANHO_MAX_NOME];
      scanf("%s",nomeA);
      add_info(h,nomeA);
    }

  }
  calcula_siglas(g);

  print_hierarquias(g);
}


/**
 * Inicializa um novo Info e atribui o nome recebido como parâmetro. Se a
 * hierarquia está vazia, adiciona no topo. Senão, procura pela próxima posição
 * vazia e adiciona o Info. Ao final retorna a Info.
 */
Info *add_info(Hierarquia *hierarquia, char *nome) {
  Info *info = (Info*) malloc(sizeof(Info));

  if (info == NULL) {
    exit(1);
  }

  create_info(info);
  strcpy(info->nome,nome);

  if (hierarquia->topo == NULL) {
    hierarquia->topo = info;
  }
  else {
    Info * aux = hierarquia->topo;
    while (aux->proximo != NULL) {
      aux = aux->proximo;
    }
    aux->proximo = info;
  }

  return info;
}


/**
 * Define quais são as siglas de cada informação dentro das hierarquias do 
 * grafo passado por parâmetro de acordo com o nome de cada uma. As siglas 
 * são únicas dentro do grafo. Por padrão a sigla é a primeira letra do nome,
 * mas se existirem nome com a mesma primeira letra, considera-se também a segunda letra
 * e assim sucetivamente.
 * 
 */
void calcula_siglas(Grafo *grafo) {
  int n = grafo->numHier;

  for (int i = 0; i < n; i++) {
    Info *aux = grafo->hierarquias[i]->topo;
    int Iaux = 0;

    while (aux != NULL) {

      int pos = 0;

      for (int j = 0; j < n; j++) {
        Info *aux2 = grafo->hierarquias[j]->topo;
        int Iaux2 = 0;
        while (aux2 != NULL) {
          if ( (i == j && Iaux2 != Iaux) || i != j ) {
            if ( strcmp(aux2->sigla, "") != 0 ) {
              char s1[TAMANHO_MAX_NOME];
              char s2[TAMANHO_MAX_NOME];
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
      char temp[TAMANHO_MAX_NOME];
      strcpy(temp,aux->nome);
      temp[pos+1] = '\0';
      strcpy(aux->sigla,temp);
      Iaux++;
      aux = aux->proximo;
    }
  }
}


/**
 * Salva os dados do grafo no arquivo de dados. Por padrão o nome do arquivo é
 * 'dados'. Significado dos caracteres especiais:
 *   '/' -- representa o término de um nome de uma dimensão/atributo;
 *   '|' -- representa o término de um campo;
 *   '*' -- representa o término de uma hierarquia;
 *   '#' -- representa o término de um registro;
 */
void save_data_to_file(Grafo *g) {
  char url[10] = "dados";
  int byteOffSet = 0;
  FILE * arq;

  arq = fopen(url, "ab");
  if (arq == NULL) {
      printf("Erro ao abrir arquivo\n");
  }
  else {
    fseek(arq, byteOffSet, SEEK_END);
    for (int i = 0; i < g->numHier; i++) {
      Info *aux = g->hierarquias[i]->topo;

      while (aux != NULL) {
        fwrite(aux->nome, 1, strlen(aux->nome), arq);
        fwrite("/", 1, 1, arq);
        fwrite(aux->sigla, 1, strlen(aux->sigla), arq);
        fwrite("|", 1, 1, arq);
        aux = aux->proximo;
      }
      fwrite("*", 1, 1, arq);
    }
    fwrite("#", 1, 1, arq);
  }

  fclose(arq);
}


/**
 * Lê as informações do arquivo de dados, a partir de um RRN de um registro.
 * Recupera as informações do registro e atribui ao parâmetro de entrada do tipo
 * Grafo.
 */
void get_data_from_file(Grafo *g, int rrn) {
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
        create_hierarquia(h);
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
          Info * aux = add_info(h,nome);

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


/**
 * A partir de um grafo de hierarquias, gera-se um grafo de derivação para
 * ser mostrado graficamente.
 */
void generate_graph_for_graphic(Grafo *grafo) {
  Vertice *topo = (Vertice*) malloc(sizeof(Vertice));

  if (topo == NULL) {
    exit(1);
  }

  create_vertice(topo);
  topo->infos = (Info**) malloc(sizeof(Info*) * grafo->numHier);
  if (topo->infos == NULL) {
    exit(1);
  }

  grafo->verticeTopo = topo;
  topo->sigla = (char*) malloc(sizeof(char) * TAMANHO_MAX_SIGLA * grafo->numHier);
  if (topo->sigla == NULL) {
    exit(1);
  }

  strcpy(topo->sigla, "");
  for (int i = 0; i < grafo->numHier; i++) {
    topo->infos[i] = grafo->hierarquias[i]->topo;
    strcat(topo->sigla, grafo->hierarquias[i]->topo->sigla);
  }

  generate_filhos(topo, grafo);
}


/**
 *
 * Função auxiliar para a função 'generate_graph_for_graphic', ela continua gerando
 * o grafo de derivação recursivamente para cada filho de vertice.
 *
 */
void generate_filhos(Vertice *vertice, Grafo *grafo) {
  for (int i =0 ; i < grafo->numHier;i++) {
    // se não percorreu toda a hierarquia
    if (vertice->infos[i] != NULL) {
      Vertice* novoVertice = (Vertice*) malloc(sizeof(Vertice));
      novoVertice->sigla = (char*) malloc( sizeof(char) * TAMANHO_MAX_SIGLA * grafo->numHier);
      if (novoVertice == NULL || novoVertice ==NULL) {
        exit(1);
      }
      create_vertice(novoVertice);
      novoVertice->infos = (Info**) malloc(sizeof(Info*) * grafo->numHier);
      if (novoVertice->infos == NULL) {
        exit(1);
      }

      strcpy(novoVertice->sigla, "");
      // copia as 'infos' (dimensao ou atributos) do vertice pai para o novo vertice filho
      for (int j=0; j< grafo->numHier; j++) {
        // a hierarquia i deve ser posta para a proxima posição
        if (j==i) {
          novoVertice->infos[j] = vertice->infos[i]->proximo;
        }
        else {
          novoVertice->infos[j] = vertice->infos[j];
        }
        //cria a sigla
        if (novoVertice->infos[j] != NULL) {
          strcat(novoVertice->sigla, novoVertice->infos[j]->sigla);
        }
      }

      if (strcmp(novoVertice->sigla,"") == 0) {
        strcpy(novoVertice->sigla,"all");
      }

      Vertice *aux = pesquisa_vertice(grafo->verticeTopo, novoVertice->sigla);
      if (aux == NULL) {
        create_aresta(vertice, novoVertice);
        if (strcmp(novoVertice->sigla,"all") != 0)
          generate_filhos(novoVertice, grafo);
      }
      else {
        create_aresta(vertice, aux);
      }

    }
  }
}


/**
 * Pesquisa um vertice descendente do parametro 'vertice' de acordo com o
 * parametro 'sigla'. Esta função é auxiliar para a função generate_filhos
 * que a usa para não gerar um novo vertice com a sigla igual a de um vertice
 * já existente.
 * 
 */
Vertice *pesquisa_vertice(Vertice *vertice, char *sigla) {
  No *aux = vertice->filhos;
  Vertice *aux2;

  if (strcmp(vertice->sigla, sigla) == 0) {
    return vertice;
  }

  while (aux != NULL) {
    if ( strcmp(aux->vertice->sigla, sigla) == 0 ) {
      return aux->vertice;
    }
    aux2 = pesquisa_vertice(aux->vertice,sigla);
    if ( aux2 != NULL ){
      return aux2;
    }
    aux = aux->proximo;
  }

  return NULL;
}


/**
 * Cria-se um arquivo '.dot' com informações do grafo de derivação recebido
 * como parâmetro. Se já existe um arquivo 'graph.dot', esse é substituído.Ao
 * final invoca o método generate_png_from_dot_file() para gerar o arquivo
 * '.png'.
 *
 * O arquivo '.dot' está no formato:
 *   digraph G {
 *     [dependência 1];
 *     [dependência 2];
 *     [dependência 3];
 *     .
 *     .
 *     .
 *   }
 */
void generate_dot_file(Grafo *grafo) {
  FILE *dot_file;
  dot_file = fopen("graph.dot", "wb");

  if (!dot_file) {
    printf("Erro na abertura do arquivo\n");
    perror("> Resultado da abertura");
    printf("> Erro: %d\n", errno);
  }
  else {
    fwrite("digraph G {", 1, strlen("digraph G {"), dot_file);
    write_all_dependencies(grafo->verticeTopo, dot_file);
    fwrite("}", 1, 1, dot_file);

    fclose(dot_file);

    // Gera o arquivo '.png'
    generate_png_from_dot_file();
  }
}


/**
 * O método percorre todos os filhos de um vértice e escreve no arquivo '.dot'
 * as dependências que estão no grafo.
 */
void write_all_dependencies(Vertice *vertice, FILE* dot_file) {
  No *aux = vertice->filhos;

  while (aux != NULL) {
    printf("\n %s -> %s", vertice->sigla, aux->vertice->sigla);
    fwrite(vertice->sigla, 1, strlen(vertice->sigla), dot_file);
    fwrite("->", 1, 2, dot_file);
    fwrite(aux->vertice->sigla, 1, strlen(aux->vertice->sigla), dot_file);
    fwrite(";\n", 1, 2, dot_file);

    write_all_dependencies(aux->vertice, dot_file);
    aux = aux->proximo;
  }
}


/**
 * Invoca o método system() para utilizar o programa 'dot' e gerar um arquivo
 * '.png' a partir de um arquivo '.dot'. Assumi-se que o arquivo '.dot' existe
 * e está na pasta corrente. Se já existe um arquivo 'graph.png', esse é
 * substituído
 */
void generate_png_from_dot_file() {
  system("dot -Tpng graph.dot > graph.png");
}


/**
 * Percorre todas as dimensões e atributos de um determinado grafo. Imprime
 * textualmente na tela.
 */
void print_hierarquias(Grafo *grafo) {
  Info *aux;

  printf("\n************************************************************************\nHierarquias:");

  for (int i = 0; i < grafo->numHier; i++) {
    aux = grafo->hierarquias[i]->topo;
    printf("\nDimensão %s(%s)",aux->nome, aux->sigla);
    aux = aux->proximo;
    while (aux != NULL ) {
      printf(" -> %s(%s)",aux->nome, aux->sigla);
      aux = aux->proximo;
    }
  }

  printf("\n************************************************************************\n\n\n");
}
