/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#ifndef IMAGEM_H
#define IMAGEM_H

typedef struct imagem_t {
    unsigned int altura;
    unsigned int largura;
    float *r;
    float *g;
    float *b;
} imagem_t;


/**
 * Retorna uma instância com todas as informações da imagem com as cores
 * separadas.
 *
 * É necessário fornecer uma função que faça a alocação de um vetor de floats
 * para cada matriz de cores da imagem.
 *
 * O usuário deverá se encarregar de liberar a memória alocada.
 */
imagem_t abrir_imagem(
    char *caminho_arquivo,
    /* Lembrete: sizeof(float) ao implementar essa função */
    float *(*alocar)(unsigned int largura, unsigned int altura)
);

/**
 * Salva a imagem do usuário num arquivo.
 */
void salvar_imagem(char *caminho_arquivo, imagem_t *imagem);


#endif /* IMAGEM_H */
