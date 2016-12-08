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

void createGrafo(Grafo *grafo) {
  grafo->hierarquias = NULL;
  grafo->numHier = 0;
}

void createHierarquia(Hierarquia* hierarquia) {
  hierarquia->topo = NULL;
}

void createInfo(Info* info) {
  info->proximo = NULL;
  strcpy(info->nome, "");
  strcpy(info->sigla, "");
}

void createVertice(Vertice * vertice) {
  vertice->infos = NULL;
  vertice->filhos = NULL;
  vertice->ancestrais = NULL;
}

Info *addInfo(Hierarquia *hierarquia, char *nome) {
  Info *info = (Info*) malloc(sizeof(Info));

  if (info == NULL) {
    exit(1);
  }

  createInfo(info);
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


void calculaSiglas(Grafo *grafo) {
  int n = grafo->numHier;

  for (int i = 0; i < n; i++) {
    Info *aux = grafo->hierarquias[i]->topo;
    int Iaux = 0;

    while (aux != NULL) {

      int pos = 0;

      for (int j=0; j<n;j++) {
        Info *aux2 = grafo->hierarquias[j]->topo;
        int Iaux2 = 0;
        while (aux2 != NULL) {
          if ( (i == j && Iaux2 != Iaux) || i != j ) {
            if ( strcmp(aux2->sigla,"") != 0 ) {
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



Vertice *pesquisaVertice(Vertice *vertice, char *sigla) {
  No *aux = vertice->filhos;
  Vertice *aux2;

  if (strcmp(vertice->sigla, sigla) == 0) {
    return vertice;
  }

  while (aux != NULL) {
    if ( strcmp(aux->vertice->sigla, sigla) == 0 ) {
      return aux->vertice;
    }
    aux2 = pesquisaVertice(aux->vertice,sigla);
    if ( aux2 != NULL ){
      return aux2;
    }
    aux = aux->proximo;
  }

  return NULL;
}

void generateGraph(Grafo *grafo) {
  Vertice *topo = (Vertice*) malloc(sizeof(Vertice));

  if (topo == NULL) {
    exit(1);
  }

  createVertice(topo);
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

  generateFilhos(topo, grafo);
}

void generateFilhos (Vertice *vertice, Grafo *grafo) {
  for (int i =0 ; i < grafo->numHier;i++) {
    // se não percorreu toda a hierarquia
    if (vertice->infos[i] != NULL) {
      Vertice* novoVertice = (Vertice*) malloc(sizeof(Vertice));
      novoVertice->sigla = (char*) malloc( sizeof(char) * TAMANHO_MAX_SIGLA * grafo->numHier);
      if (novoVertice == NULL || novoVertice ==NULL) {
        exit(1);
      }
      createVertice(novoVertice);
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

      Vertice *aux = pesquisaVertice(grafo->verticeTopo, novoVertice->sigla);
      if (aux == NULL) {
        createAresta(vertice, novoVertice);
        if (strcmp(novoVertice->sigla,"all") != 0)
          generateFilhos(novoVertice, grafo);
      }
      else {
        createAresta(vertice, aux);
      }

    }
  }
}

void createAresta(Vertice *inicio, Vertice *fim) {
  No *filho = (No*) malloc(sizeof (No));
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
 * Cria-se um arquivo '.dot' com informações do grafo de derivação recebido
 * como parâmetro. Ao final invoca o método generate_png_from_dot_file() para
 * gerar o arquivo '.png'.
 * O arquivo '.dot' está no formato:
 *   digraph G {
 *     [relação 1];
 *     [relação 2];
 *     [relação 3];
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
    percorreGrafo(grafo->verticeTopo, dot_file);
    fwrite("}", 1, 1, dot_file);

    fclose(dot_file);

    // Gera o arquivo '.png'
    generate_png_from_dot_file();
  }
}

/**
 * Invoca o método system() para utilizar o programa 'dot' e gerar um arquivo
 * '.png' a partir de um arquivo '.dot'. Assumi-se que o arquivo '.dot' existe
 * e está na pasta corrente.
 */
void generate_png_from_dot_file() {
  system("dot -Tpng graph.dot > graph.png");
}

void percorreGrafo (Vertice *v, FILE* dot_file) {
  No * aux = v->filhos;
  while (aux != NULL) {
    printf("\n %s -> %s", v->sigla, aux->vertice->sigla);
    fwrite(v->sigla, 1, strlen(v->sigla), dot_file);
    fwrite("->", 1, 2, dot_file);
    fwrite(aux->vertice->sigla, 1, strlen(aux->vertice->sigla), dot_file);
    fwrite(";\n", 1, 2, dot_file);

    percorreGrafo(aux->vertice,dot_file);
    aux = aux->proximo;
  }
}



