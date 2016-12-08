#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"
#include <errno.h>

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

void createVertice(Vertice * v) {
  v->infos = NULL;
  v->filhos = NULL;
  v->ancestrais = NULL;
}


Vertice * pesquisaVertice( Vertice* v, char* sigla) {
  No * aux = v->filhos;
  Vertice * aux2;
  if (strcmp(v->sigla, sigla) == 0) {
    return v;
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

void generateGraph(Grafo *g) {
  Vertice * topo = (Vertice*) malloc(sizeof(Vertice));
  if (topo == NULL) {
    exit(1);
  }
  createVertice(topo);
  topo->infos = (Info**) malloc(sizeof(Info*) * g->numHier);
  if (topo->infos == NULL) {
    exit(1);
  }
  g->verticeTopo = topo;
  topo->sigla = (char*) malloc(sizeof(char) * tamSigla * g->numHier);
  if (topo->sigla == NULL) {
    exit(1);
  }
  strcpy(topo->sigla, "");
  for (int i =0; i < g->numHier;i ++) {
    topo->infos[i] = g->hierarquias[i]->topo;
    strcat(topo->sigla, g->hierarquias[i]->topo->sigla);
  }
  generateFilhos(topo, g);


}

void generateFilhos (Vertice * v, Grafo *g) {
  for (int i =0 ; i < g->numHier;i++) {
    // se não percorreu toda a hierarquia
    if (v->infos[i] != NULL) {
      Vertice* novoVertice = (Vertice*) malloc(sizeof(Vertice));
      novoVertice->sigla = (char*) malloc( sizeof(char) * tamSigla * g->numHier);
      if (novoVertice == NULL || novoVertice ==NULL) {
        exit(1);
      }
      createVertice(novoVertice);
      novoVertice->infos = (Info**) malloc(sizeof(Info*) * g->numHier); 
      if (novoVertice->infos == NULL) {
        exit(1);
      }
      
      strcpy(novoVertice->sigla, "");
      // copia as 'infos' (dimensao ou atributos) do vertice pai para o novo vertice filho
      for (int j=0; j< g->numHier; j++) {
        // a hierarquia i deve ser posta para a proxima posição
        if (j==i) {
          novoVertice->infos[j] = v->infos[i]->proximo;
//          printf("Q%s",v->infos[i]->sigla) ;
          //printf("\n|||Sigla%s",v->infos[j]->sigla);
        }
        else {
          novoVertice->infos[j] = v->infos[j];
        }
        //cria a sigla
        if (novoVertice->infos[j] != NULL) {    
          strcat(novoVertice->sigla, novoVertice->infos[j]->sigla);
        }
      }
      
      if (strcmp(novoVertice->sigla,"") == 0) {
        strcpy(novoVertice->sigla,"all");
      }

      Vertice * aux = pesquisaVertice(g->verticeTopo, novoVertice->sigla);
      if (aux == NULL) {
        criaAresta(v,novoVertice);
        if (strcmp(novoVertice->sigla,"all") != 0) 
          generateFilhos(novoVertice,g);
      }
      else {
        criaAresta(v,aux);
      }

    }
  }
}



void criaAresta (Vertice * inicio, Vertice* fim) {
  
  No * filho = (No*) malloc(sizeof (No));
  No * ancestral = (No*) malloc(sizeof(No));
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

void geraDot(Grafo * g) {
  
  FILE * dot_file;
  dot_file = fopen("graph.dot", "wb");
  if (!dot_file) {
      printf("Erro na abertura do dot_file\n");
      perror("> Resultado da abertura");
      printf("> Erro: %d\n", errno);
    }
    else {
      fwrite("digraph G {", 1, strlen("digraph G {"), dot_file);
    percorreGrafo(g->verticeTopo,dot_file);
      fwrite("}", 1, 1, dot_file);
      fclose(dot_file);
      // Gera o arquivo '.png' a partir do arquivo '.dot'
        system("dot -Tpng graph.dot > graph.png");
    }
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



