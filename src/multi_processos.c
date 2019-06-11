/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include "imagem.h"
#include "processamento.h"
#include "metodo.h"

#define N_PROCESSOS 3
#define TAM_BATCH 3000


void *_mmap(int _sizeof) {
    /* Definir flags de protecao e visibilidade de memoria */
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;

    /* Tentamos alocar memória de maneira segura */
    void *memoria = (float*) mmap(
        NULL, _sizeof, protection, visibility, 0, 0
    );
    /* Conferimos se houve sucesso */
    if (memoria == MAP_FAILED) {
        fprintf(stderr, "Falha ao alocar memória compartilhada.\n");
        exit(1);
    }

    /* Retornamos a memória alocada */
    return memoria;
}


float *alocar(unsigned int altura, unsigned int largura) {
    /* Utilizamos nosso método genérico e seguro */
    return (float *) _mmap(sizeof(float) * altura * largura);
}


void processar_imagem(char *imagem_entrada, char *imagem_saida) {
    /* Abrimos a imagem */
    imagem_t imagem = abrir_imagem(imagem_entrada, alocar);

    /* Alocamos 3 matrizes que serão utilizadas para aplicar o filtro blur */
    float *red = alocar(imagem.altura, imagem.largura);
    float *green = alocar(imagem.altura, imagem.largura);
    float *blue = alocar(imagem.altura, imagem.largura);

    /* Variáveis que armazenam a posição dentro da imagem */
    int *n_coluna = (int *) _mmap(sizeof(int));
    (*n_coluna) = 0;
    int *n_linha = (int *) _mmap(sizeof(int));
    (*n_linha) = 0;

    pid_t pid[N_PROCESSOS];
    int pipefd[2];
    pipe(pipefd);

    for (int i = 0; i < N_PROCESSOS; i++) {
        pid[i] = fork();
        /* Processo filho */
        if (pid[i] == 0){
        	batch_t batch;
		    close(pipefd[1]);

            /* Enquanto houver batches no pipe, fazemos a leitura */
		    while(read(pipefd[0], &batch, sizeof(batch_t)) > 0) {

	            /* Fazemos para a cor vermelha */
	            batch.matriz = imagem.r;
	            processar_pixels(red, &batch, aplicar_blur);

	            /* Fazemos para a cor verde */
	            batch.matriz = imagem.g;
	            processar_pixels(green, &batch, aplicar_blur);

	            /* Fazemos para a cor azul */
	            batch.matriz = imagem.b;
	            processar_pixels(blue, &batch, aplicar_blur);

		    }
 
            exit(0);
        }
    }

    /* Processo pai */
    close(pipefd[0]);

    int j = 0;
    batch_t *batches = _mmap(
        (((imagem.largura * imagem.altura) / TAM_BATCH) + 1) * sizeof(batch_t)
    );

    /* Enquanto há batch para colocar no pipe */
    while ((*n_linha) < imagem.altura){
        /* Inicializamos um novo batch */
	    batch_t *batch = &(batches[j++]);
        batch->coluna = (*n_coluna);
        batch->linha = (*n_linha);
        batch->numero_pixels = TAM_BATCH;
        batch->altura = imagem.altura;
        batch->largura = imagem.largura;

        /* Escrevemos o batch no pipe */
    	write(pipefd[1], (const void *) batch, sizeof(batch_t));
        
        /* Atualizamos a posição na imagem */
        (*n_coluna) += TAM_BATCH;
        while ((*n_coluna) >= imagem.largura) {
            (*n_coluna) -= imagem.largura;
            (*n_linha) += 1;
        }
    }

    close(pipefd[1]);

     for (int i = 0; i < N_PROCESSOS; i++) {
        waitpid(pid[i], NULL, 0);
     }

    /* Passamos as matrizes red, green e blue para a imagem */
    imagem.r = red;
    imagem.g = green;
    imagem.b = blue;


    /* Salvamos a imagem */
    salvar_imagem(imagem_saida, &imagem);
}
