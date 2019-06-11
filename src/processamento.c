/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#include <stdio.h>
#include "processamento.h"


void processar_pixels(
    float *matriz,
    batch_t *batch,
    float (*efeito)(
        float *matriz_leitura,
        unsigned int altura, unsigned int largura,
        unsigned int linha, unsigned int coluna
    )
) {
    unsigned int linha = batch->linha;
    unsigned int coluna = batch->coluna;

    /* Enquanto ainda temos pixels para processar */
    int numero_pixels = batch->numero_pixels;
    while (numero_pixels > 0) {

        /* Aplicamos o efeito no pixel atual */
        unsigned int posicao = (linha * batch->largura) + coluna;
        matriz[posicao] = efeito(
            batch->matriz,
            batch->altura, batch->largura,
            linha, coluna
        );

        /* Agora que terminamos esse pixel, vamos ao próximo */
        numero_pixels--;
        coluna++;
        /* Conferimos se saímos da imagem */
        if (coluna > batch->largura) {
            coluna = 0;
            linha++;
        }
        if (linha > batch->altura && numero_pixels > 0) {
            fprintf(
                stderr, "Falha de cálculo: imagem terminou antes do batch.\n"
            );
            break;
        }
    }
}


float aplicar_blur(
    float *matriz,
    unsigned int altura, unsigned int largura,
    unsigned int linha, unsigned int coluna
) {
    /* Raio do efeito, declarado no header */
    int r = RAIO_EFEITO;

    /* O blur é um efeito que funciona através da média, então guardaremos a
     * soma no acumulador e o número de itens somados */
    double acumulador = 0;
    unsigned int somas_realizadas = 0;

    /* Percorremos em volta do pixel que estamos calculando */
    /* Percorremos em torno da posição verticalmente */
    for (int d_linha = -r; d_linha < r; d_linha++) {
        /* calculamos a posição da linha atual */
        int posicao_linha = linha + d_linha;

        /* Verificamos se o pixel não existe */
        if (posicao_linha < 0) {
            /* Continuamos para a próxima posição da linha */
            continue;
        } else if (posicao_linha >= altura) {
            /* Como saímos da matriz, já podemos parar */
            break;
        }

        /* Percorremos em torno da posição horizontalmente */
        for (int d_coluna = -r; d_coluna < r; d_coluna++) {
            int posicao_coluna = coluna + d_coluna;

            /* Verificamos se o pixel não existe */
            if (posicao_coluna < 0) {
                /* Continuamos para a próxima posição da coluna */
                continue;
            } else if (posicao_coluna >= largura) {
                /* Como saímos da matriz, já podemos parar */
                break;
            }

            /* Aqui estamos numa posição em que o pixel existe, utilizamos na
             * soma */
            acumulador += matriz[posicao_coluna + (posicao_linha * largura)];
            somas_realizadas++;
        }
    }

    /* Como temos tudo, basta calcular a média: soma / número de itens.
     * Utilizo double para tornar o erro de aproximação menor */
    return (float) (acumulador / (double) somas_realizadas);
}


float aplicar_nada(
    float *matriz,
    unsigned int altura, unsigned int largura,
    unsigned int linha, unsigned int coluna
) {
    return matriz[(linha * largura) + coluna];
}


float aplicar_branco(
    float *matriz,
    unsigned int altura, unsigned int largura,
    unsigned int linha, unsigned int coluna
) {
    return 255.0;
}
