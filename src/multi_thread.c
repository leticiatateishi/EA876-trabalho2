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

#define TAM_FILA 10
#define N_CONSUMIDORES 3
#define TAM_BATCH 30

batch_t fila[TAM_FILA];
pthread_mutex_t trava, tem_espaco, tem_dados;
int n_fila = 0;
int n_escrita, n_leitura = 0;

void escrever(batch_t batch){
	pthread_mutex_lock(&tem_espaco);
	pthread_mutex_lock(&trava);

	if (n_escrita >= TAM_FILA)
		n_escrita -= TAM_FILA;

	fila[n_escrita] = batch;
	n_escrita ++;
	n_fila ++;

	pthread_mutex_unlock(&tem_dados);

	if (n_elementos < TAM_FILA) 
		pthread_mutex_unlock(&tem_espaco);

	pthread_mutex_unlock(&trava);
}

batch_t ler() {
	batch_t batch;

	pthread_mutex_lock(&tem_dados);
	pthread_mutex_lock(&trava);
	batch = fila[n_leitura];
	n_leitura++;
	n_fila--;
	if (n_leitura >= TAM_FILA)
		n_leitura -= TAM_FILA;

	pthread_mutex_unlock(&tem_espaco);

	if (n_elementos > 0) 
		pthread_mutex_unlock(&tem_dados);

	pthread_mutex_unlock(&trava);

	return batch;
}

void *produtor(void *arg){

	imagem_t imagem = (imagem_t) arg;

	batch_t batch;
    /* Trabalho que começará do pixel (0, 0) e terá o tamanho da imagem */
    batch.coluna = 0;
    batch.linha = 0;

    while(1){

    	batch.largura = (batch.coluna) + TAM_BATCH;
    	batch.altura = (batch.linha) + TAM_BATCH;
    	if(batch.largura > imagem.largura)
    		batch.largura = imagem.largura;
    	else if(batch.largura < 0)
    		batch.largura = 0;
    	if (batch.altura > imagem.altura)
    		batch.altura = imagem.altura;
    	else if(batch.altura < 0)
    		batch.altura = 0;

    	escrever(batch);
    	batch.coluna = batch.largura + 1;
    	// batch.linha = batch.altura + 1;
    }


}

void *consumidor(void *arg){

}

void processar_imagem(char *imagem_entrada, char *imagem_saida) {
    /* Abrimos a imagem */
    imagem_t imagem = abrir_imagem(imagem_entrada, alocar);

    pthread_t prod;
    pthread_t cons[N_CONSUMIDORES];

	pthread_create(&prod, NULL, produtor, NULL);
	for (int i = 0; i < N_CONSUMIDORES; i++)
		pthread_create(&(cons[i]), NULL, consumidor, (void*) imagem);

	for (int i = 0; i < N_CONSUMIDORES; i++) {
 	   pthread_join(cons[i], NULL);

    /* Salvamos a imagem */
    salvar_imagem(imagem_saida, &imagem);

    /* Antes de retornar, liberamos memória */
    // TODO
}
