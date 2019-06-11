/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#include <stdio.h>
#include <stdlib.h>
#include <FreeImage.h>

#include "imagem.h"


imagem_t abrir_imagem(
    char *caminho_arquivo,
    float *(*alocar)(unsigned int largura, unsigned int altura)
) {
    /* Abrimos imagem */
    FIBITMAP *bitmap = FreeImage_Load(FIF_JPEG, caminho_arquivo, 0);
    if (bitmap == 0) {
        fprintf(stderr, "Não encontrei imagem em \"%s\"\n", caminho_arquivo);
    }

    /* Pegamos propriedades */
    unsigned int largura = FreeImage_GetWidth(bitmap);
    unsigned int altura = FreeImage_GetHeight(bitmap);

    /* Criamos nossa instância */
    imagem_t imagem;
    imagem.largura = largura;
    imagem.altura = altura;
    imagem.r = alocar(largura, altura);
    imagem.g = alocar(largura, altura);
    imagem.b = alocar(largura, altura);

    /* Copiamos a imgem */
    for (unsigned int linha = 0; linha < altura; linha++) {
        for (unsigned int coluna = 0; coluna < largura; coluna++) {
            /* Pegamos a cor do pixel atual */
            RGBQUAD cor;
            FreeImage_GetPixelColor(bitmap, coluna, linha, &cor);

            /* Colocamos a respectiva cor no local apropriado */
            unsigned int posicao = (linha * largura) + coluna;
            imagem.r[posicao] = cor.rgbRed;
            imagem.g[posicao] = cor.rgbGreen;
            imagem.b[posicao] = cor.rgbBlue;
        }
    }

    /* Descarregamos a imagem */
    FreeImage_Unload(bitmap);
    /* Retornamos a nossa instância */
    return imagem;
}


void salvar_imagem(char *caminho_arquivo, imagem_t *imagem) {
    /* Alocamos espaço para a imagem */
    FIBITMAP *bitmap = FreeImage_Allocate(
        imagem->largura,
        imagem->altura,
        24, 0, 0, 0
    );

    /* Copiamos a imgem */
    for (unsigned int linha = 0; linha < imagem->altura; linha++) {
        for (unsigned int coluna = 0; coluna < imagem->largura; coluna++) {
            unsigned int posicao = (linha * imagem->largura) + coluna;

            RGBQUAD cor;
            cor.rgbRed = imagem->r[posicao];
            cor.rgbGreen = imagem->g[posicao];
            cor.rgbBlue = imagem->b[posicao];

            FreeImage_SetPixelColor(bitmap, coluna, linha, &cor);
        }
    }

    /* Salvamos a imagem atual */
    FreeImage_Save(FIF_JPEG, bitmap, caminho_arquivo, JPEG_DEFAULT);
    /* Desalocamos o bitmap */
    FreeImage_Unload(bitmap);
}

