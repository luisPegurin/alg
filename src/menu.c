#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "structs.h"
#include "menu.h"

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
  printf("\nDigite o número id do grafo: ");
  int n;
  scanf("%d",&n);
  /*
  Grafo grafo;
  FILE *dot_file;
  char *string_aux;

  createGrafo(&grafo);
  leDados(&grafo, 0);

  if (grafo.numHier == 0) {
    printf("\nNão existe esse grafo");
  }
  else {
    dot_file = fopen("graph.dot", "wb");

    if (!dot_file) {
      printf("Erro na abertura do dot_file\n");
      perror("> Resultado da abertura");
      printf("> Erro: %d\n", errno);
    }
    else {

      // Começa a escrever o digrafo
      fwrite("digraph G {", 1, strlen("digraph G {"), dot_file);
      
      for (int i = 0; i < grafo.numHier; i++) {
        Info *aux = grafo.hierarquias[i]->topo;

        // Escreve a maior granularidade
        if (aux->proximo != NULL)
        {
          fwrite ("all", 1, strlen("all"), dot_file);
          fwrite ("->", 1, 2, dot_file);
          fwrite (aux->proximo->nome, 1, strlen(aux->proximo->nome), dot_file);
          fwrite (";", 1, 1, dot_file);
        }

        aux = aux->proximo;

        // Escreve as próximas granularidades
        while (aux->proximo != NULL) {
          fwrite (aux->nome, 1, strlen(aux->nome), dot_file);
          fwrite ("->", 1, 2, dot_file);
          fwrite (aux->proximo->nome, 1, strlen(aux->proximo->nome), dot_file);
          fwrite (";", 1, 1, dot_file);
          aux = aux->proximo;
        }

      }

      // Fecha o digrafo
      fwrite("}", 1, 1, dot_file);

      fclose(dot_file);

      // Gera o arquivo '.png' a partir do arquivo '.dot'
      system("dot -Tpng graph.dot > graph.png");
    }

  }*/
  Grafo grafo;
  createGrafo(&grafo);
  leDados(&grafo, n);
  generateGraph(&grafo);
  geraDot(&grafo);

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