/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "imagem.h"
#include "processamento.h"
#include "metodo.h"


/** Tamanho do buffer entre produtor e consumidor */
#define TAM_FILA 512
/** Número de threads consumidoras */
#define N_CONSUMIDORES 3

#define TAM_BATCH 5000


/**
 * Função que escreve nas variáveis globais a próxima estrutura a ser processada
 * pelos consumidores.
 */
void escrever(batch_t batch);

/**
 * Função que lê, a partir de variáveis globais, a próxima estrutura a ser
 * processada pelos consumidores.
 */
batch_t ler();

/**
 * O produtor separa a imagem em batchs de mesmo tamanho (exceto possivelmente
 * o último) e os insere no vetor circular com a função escrever
 */
void *produtor(void *arg);

/**
 * O consumidor lê vários batches do vetor circular e os processa
 */
void *consumidor(void *arg);


/** Vetor circular para o buffer */
batch_t fila[TAM_FILA];
pthread_mutex_t trava, tem_espaco, tem_dados;
unsigned int n_fila = 0;
unsigned int n_escrita, n_leitura = 0;
/** Imagem finalizada */
float *red, *green, *blue;


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

    /* Alocamos uma matriz para cada cor */
    red = alocar(imagem.altura, imagem.largura);
    green = alocar(imagem.altura, imagem.largura);
    blue = alocar(imagem.altura, imagem.largura);

    pthread_t prod;
    pthread_t cons[N_CONSUMIDORES];

    pthread_create(&prod, NULL, produtor, (void*) &imagem);
    for (int i = 0; i < N_CONSUMIDORES; i++) {
        pthread_create(&(cons[i]), NULL, consumidor, (void*) &imagem);
    }

    for (int i = 0; i < N_CONSUMIDORES; i++) {
        pthread_join(cons[i], NULL);
    }

     /* Passamos as matrizes red, green e blue para a imagem */
    free(imagem.r);
    imagem.r = red;
    free(imagem.g);
    imagem.g = green;
    free(imagem.b);
    imagem.b = blue;

    /* Salvamos a imagem */
    salvar_imagem(imagem_saida, &imagem);

    /* Antes de retornar, liberamos memória */
    free(imagem.r);
    free(imagem.g);
    free(imagem.b);
}


void escrever(batch_t batch) {
    pthread_mutex_lock(&tem_espaco);
    pthread_mutex_lock(&trava);

    if (n_escrita >= TAM_FILA) {
        n_escrita -= TAM_FILA;
    }

    /* Inserimos um batch em um vetor circular */
    fila[n_escrita] = batch;
    n_escrita ++;
    n_fila ++;

    pthread_mutex_unlock(&tem_dados);

    if (n_fila < TAM_FILA) {
        pthread_mutex_unlock(&tem_espaco);
    }

    pthread_mutex_unlock(&trava);
}


batch_t ler() {
    batch_t batch;

    pthread_mutex_lock(&tem_dados);
    pthread_mutex_lock(&trava);

    /* Lemos um batch do vetor circular */
    batch = fila[n_leitura];
    n_leitura++;
    n_fila--;
    if (n_leitura >= TAM_FILA) {
        n_leitura -= TAM_FILA;
    }

    pthread_mutex_unlock(&tem_espaco);

    if (n_fila > 0) {
        pthread_mutex_unlock(&tem_dados);
    }

    pthread_mutex_unlock(&trava);

    return batch;
}


void *produtor(void *arg) {
    imagem_t *imagem = (imagem_t*) arg;

    batch_t batch;
    /* Trabalho que começará do pixel (0, 0) e terá o tamanho da imagem */
    batch.coluna = 0;
    batch.linha = 0;
    batch.numero_pixels = TAM_BATCH;
    batch.altura = imagem->altura;
    batch.largura = imagem->largura;

    while (1) {
        /* Escrevemos o batch preparado */
        escrever(batch);

        /* Calculamos o ponto de término, consideramos o tamanho do batch
         * decrescido de 1 pois o índice começa de zero. Sem isso, a imagem
         * possuiria pontos pretos espalhados */
        batch.coluna += batch.numero_pixels;
        while (batch.coluna >= imagem->largura) {
            batch.linha += 1;
            batch.coluna -= imagem->largura;
        }

        /* Verificamos se terminamos a imagem */
        if (batch.linha >= imagem->altura) {
            break;
        }
    }

    /* Escrevemos alguns batches para terminar as threads consumidoras */
    for (int i = 0; i < N_CONSUMIDORES; i++) {
        batch.numero_pixels = 0;
        escrever(batch);
    }

    return NULL;
}


void *consumidor(void *arg) {
    imagem_t *imagem = (imagem_t*) arg;
    batch_t batch;

    /* Cada thread irá consumir indefinidamente (até pegar um batch sem
     * pixels) */
    while ((batch = ler()).numero_pixels > 0) {
        /* Fazemos para a cor vermelha */
        batch.matriz = imagem->r;
        processar_pixels(red, &batch, aplicar_blur);

        /* Fazemos para a cor verde */
        batch.matriz = imagem->g;
        processar_pixels(green, &batch, aplicar_blur);

        /* Fazemos para a cor azul */
        batch.matriz = imagem->b;
        processar_pixels(blue, &batch, aplicar_blur);
    }

    return NULL;
}
