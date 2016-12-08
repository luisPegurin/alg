/**
 * menu.h -- Definição dos métodos do menu
 *
 * Autores:
 *  Luís Henrique Pegurin 9313325
 *  Mateus Medeiros       9266410
 *  Guilherme Kayo Shida  6878696
 */

#ifndef MENU_H_
#define MENU_H_

/**
 * @brief Método para mostrar o menu principal
 *
 * @return Void
 */
void menu_principal();

/**
 * @brief Método para voltar a menu principal ao pressionar [ENTER]
 *
 * @return Void
 */
void menu_continuar();

/**
 * @brief Método para adicionar um novo grafo
 *
 * @return Void
 */
void menu_novo_grafo();

/**
 * @brief Método para visualizar os registros armazenados
 *
 * @return Void
 */
void menu_visualizar_grafo();

/**
 * @brief Método para gerar o arquivo de visualização gráfica
 *
 * @return Void
 */
void menu_visualizacao_grafica();

/**
 * @brief Método para sair do programa
 *
 * @return Void
 */
void menu_sair();

#endif
