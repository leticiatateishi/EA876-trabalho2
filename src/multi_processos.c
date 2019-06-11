/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "imagem.h"
#include "processamento.h"
#include "metodo.h"

#define N_PROCESSOS 100
#define TAM_BATCH 3000


float *alocar(unsigned int altura, unsigned int largura) {
    /* Definir flags de protecao e visibilidade de memoria */
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;

    float *memoria = (float*) mmap(
        NULL, sizeof(float)*altura*largura, protection, visibility, 0, 0
    );

    return memoria;
}


void processar_imagem(char *imagem_entrada, char *imagem_saida) {
    /* Abrimos a imagem */
    imagem_t imagem = abrir_imagem(imagem_entrada, alocar);

    float *red = alocar(imagem.altura, imagem.largura);
    float *green = alocar(imagem.altura, imagem.largura);
    float *blue = alocar(imagem.altura, imagem.largura);

    int *n_coluna = alocar(1, 1);
    int *n_linha = alocar(1, 1);

    pid_t pid[N_PROCESSOS];
    for (int i = 0; i < N_PROCESSOS; i++) {
        pid[i] = fork();
        if (pid[i] == 0){

            batch_t batch;
            batch.coluna = (*n_coluna);
            batch.linha = (*n_linha);
            batch.numero_pixels = TAM_BATCH;
            batch.altura = imagem.altura;
            batch.largura = imagem.largura;

            if ((batch.coluna + TAM_BATCH) > imagem.largura) {

                /* Consideramos a possibilidade do ultimo batch ser menor do
                 * que os outros, caso o número de pixels não seja múltiplo de
                 * TAM_BATCH */
                if (batch.linha == imagem.altura) {
                    batch.numero_pixels =
                        imagem.largura - (batch.coluna - TAM_BATCH);
                }
            }

            /* Fazemos para a cor vermelha */
            batch.matriz = imagem.r;
            processar_pixels(red, &batch, aplicar_branco);

            /* Fazemos para a cor verde */
            batch.matriz = imagem.g;
            processar_pixels(green, &batch, aplicar_branco);

            /* Fazemos para a cor azul */
            batch.matriz = imagem.b;
            processar_pixels(blue, &batch, aplicar_branco);


            (*n_coluna) += TAM_BATCH;
            if ((*n_coluna) > imagem.largura) {
                (*n_coluna) %= imagem.largura;
                (*n_linha) += 1;
            }

            exit(0);
        }
    }

     for (int i = 0; i < N_PROCESSOS; i++) {
        waitpid(pid[i], NULL, 0);
     }

    /* Passamos as matrizes red, green e blue para a imagem */
    // free(imagem.r);
    imagem.r = red;
    // free(imagem.g);
    imagem.g = green;
    // free(imagem.b);
    imagem.b = blue;


    /* Salvamos a imagem */
    salvar_imagem(imagem_saida, &imagem);

    /* Antes de retornar, liberamos memória */
    // TODO
}
