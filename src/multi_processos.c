/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#include <stdlib.h>
#include <stdio.h>

#include "imagem.h"
#include "processamento.h"
#include "metodo.h"


float *alocar(unsigned int altura, unsigned int largura) {
    // TODO
    return NULL;
}


void processar_imagem(char *imagem_entrada, char *imagem_saida) {
    /* Abrimos a imagem */
    imagem_t imagem = abrir_imagem(imagem_entrada, alocar);

    
	// /* Definir flags de protecao e visibilidade de memoria */
	// int protection = PROT_READ | PROT_WRITE;
	// int visibility = MAP_SHARED | MAP_ANON;

	// int **memoria = (int**) mmap(NULL, sizeof(int*), protection, visibility, 0, 0);

    /* Salvamos a imagem */
    salvar_imagem(imagem_saida, &imagem);

    /* Antes de retornar, liberamos memória */
    // TODO
}
