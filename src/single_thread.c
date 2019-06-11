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
    /* Alocamos memória suficiente para a imagem */
    float *memoria = malloc(altura * largura * sizeof(float));

    /* Conferimos se foi bem sucedida a alocação */
    if (memoria == NULL) {
        fprintf(stderr, "Falha ao alocar memória para a imagem.\n");
        exit(1);
    }

    return memoria;
}


void processar_imagem(char *imagem_entrada, char *imagem_saida) {
    /* Abrimos a imagem */
    imagem_t imagem = abrir_imagem(imagem_entrada, alocar);

    /* Fazemos um trabalho a ser executado nesta mesma thread, ele fará toda
     * a imagem */
    batch_t batch;
    /* Trabalho que começará do pixel (0, 0) e terá o tamanho da imagem */
    batch.coluna = 0;
    batch.linha = 0;
    batch.numero_pixels = imagem.largura * imagem.altura;
    /* Passamos a imagem */
    batch.largura = imagem.largura;
    batch.altura = imagem.altura;

    /* Fazemos para a cor vermelha */
    batch.matriz = imagem.r;
    float *r = alocar(imagem.altura, imagem.largura);
    processar_pixels(r, &batch, aplicar_blur);

    /* Fazemos para a cor verde */
    batch.matriz = imagem.g;
    float *g = alocar(imagem.altura, imagem.largura);
    processar_pixels(g, &batch, aplicar_blur);

    /* Fazemos para a cor azul */
    batch.matriz = imagem.b;
    float *b = alocar(imagem.altura, imagem.largura);
    processar_pixels(b, &batch, aplicar_blur);

    /* Substituímos a imagem anterior pela nossa */
    free(imagem.r);
    imagem.r = r;
    free(imagem.g);
    imagem.g = g;
    free(imagem.b);
    imagem.b = b;

    /* Salvamos a imagem */
    salvar_imagem(imagem_saida, &imagem);

    /* Antes de retornar, liberamos memória */
    free(imagem.r);
    free(imagem.g);
    free(imagem.b);
}
