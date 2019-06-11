/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

/** Tamanho do efeito aplicado na imagem */
#define RAIO_EFEITO     10


/** Conjunto de pixels a serem processados */
typedef struct batch_t {
    /* Do trabalho (batch) a fazer */
    unsigned int numero_pixels;
    unsigned int linha;
    unsigned int coluna;
    /* Da imagem */
    unsigned int altura;
    unsigned int largura;
    float *matriz;
} batch_t;


/**
 * Recebe um batch de pixels e aplica o efeito dado em cada um deles, escrevendo
 * o resultado na posição dada na matriz final.
 */
void processar_pixels(
    float *matriz,
    batch_t *batch,
    float (*efeito)(
        float *matriz_leitura,
        unsigned int altura, unsigned int largura,
        unsigned int linha, unsigned int coluna
    )
);


/**
 * Aplica blur no pixel de posição dada na matriz de cor única.
 *
 * Retorna o valor final do pixel.
 */
float aplicar_blur(
    float *matriz_leitura,
    unsigned int altura, unsigned int largura,
    unsigned int linha, unsigned int coluna
);

/**
 * Retorna o valor original do pixel recebido.
 */
float aplicar_nada(
    float *matriz_leitura,
    unsigned int altura, unsigned int largura,
    unsigned int linha, unsigned int coluna
);

float aplicar_branco(
    float *matriz,
    unsigned int altura, unsigned int largura,
    unsigned int linha, unsigned int coluna
);

#endif /* PROCESSAMENTO_H */
