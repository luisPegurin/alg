/**
 * menu.c -- Implementação do menu
 *
 * Autores:
 *  Luís Henrique Pegurin 9313325
 *  Mateus Medeiros       9266410
 *  Guilherme Kayo Shida  6878696
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.h"
#include "menu.h"

/**
 * O método mostra uma as opções do menu principal que o usuário pode escolher.
 * Ao escolher uma opção, um método específico para cada opção é invocado.
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
    switch (menu_opcao) {
      case 1:
        menu_visualizar_grafo();
      case 2:
        menu_novo_grafo();
      case 3:
        menu_visualizacao_grafica();
      case 4:
        menu_sair();
    }
  }
}

/**
 * O método espera o usuário a apertar [ENTER]. Ao apertar [ENTER] o método
 * menu_principal é invocado.
 */
void menu_continuar() {
  printf("Aperte [ENTER] para continuar\n");
  while( getchar() != '\n' );
  getchar();

  menu_principal();
}

/**
 * O método inicializa um grafo. Coleta as informações e salva no arquivo de
 * registros.
 */
void menu_novo_grafo() {
  Grafo g;

  createGrafo(&g);
  novoGrafo(&g);
  armazenaDados(&g);

  menu_continuar();
}

/**
 * O método busca uma hierarquia a partir de um id. Se o grafo existe imprime o
 * registro.
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
 * O método busca a hierarquia a partir de um id. Com as informações do grafo,
 * gera-se o arquivo do tipo '.dot'.
 */
void menu_visualizacao_grafica() {
  Grafo grafo;
  int id_grafo;

  printf("\nDigite o id do grafo: ");
  scanf("%d", &id_grafo);

  createGrafo(&grafo);
  leDados(&grafo, id_grafo);
  generateGraph(&grafo);
  generate_dot_file(&grafo);

  menu_continuar();
}

/**
 * O método avisa ao usuário que o programa será encerrado. Depois de 2
 * segundos o programa é fechado.
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
