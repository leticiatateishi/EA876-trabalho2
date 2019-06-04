/**
 * Rafael Sartori M. Santos, 186154
 * Letícia Mayumi Araújo Tateishi, 201454
 *
 * Trabalho 2 de EA876 1s2019 na Unicamp
 */

#include <stdio.h>
#include "metodo.h"


int main(int numero_argumentos, char **argumentos) {
    /* Conferimos se o uso foi correto */
    if ((numero_argumentos - 1) != 2) {
        printf(
            "Uso: (programa) (caminho para imagem de entrada)"
            " (caminho para imagem de saída)\n"
        );
        return -1;
    }

    /* Pegamos os caminhos dos arquivos a serem processados */
    char *imagem_entrada = argumentos[1];
    char *imagem_saida = argumentos[2];

    /* Processamos a imagem utilizando o método que queremos */
    processar_imagem(imagem_entrada, imagem_saida);

    /* Retornamos */
    return 0;
}
