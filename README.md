# Trabalho 2 - EA876 - 1s2019

Por Letícia Tateishi (201454) e Rafael Sartori (186154), disponível em [repositório no GitHub](https://github.com/rafaelsartori96/EA876-trabalho2).

Este trabalho tem como fim avaliar a performance de programas _multithread_, multiprocessos e _singlethread_ entre si utilizando como contexto o processamento de imagens.


## Organização do projeto

Cada conjunto _header/source_ é compilado separadamente. A ligação é feita utilizando todos os conjuntos abaixo com apenas um objeto (`.o`) que implementa `metodo.h`.

_Header/source_ | Utilização no projeto
--------------- | -------------
`imagem.(h/c)` | código para carregar imagens à memória e salvá-las novamente ao disco.
`processamento.(h/c)` | Biblioteca para aplicar efeitos em imagens na memória de acordo com uma estrutura que divide cada imagem em pedaços de tamanho variável.
`main.c` | Interface de interação com o usuário para verificar o caminho de entrada/saída da imagem a ser processada.
`metodo.h` | Cabeçalho para ser implementado por cada tipo de programa (`single_thread.c` para uma thread, `multi_thread.c` para várias threads e `multi_processo.c` para vários processos).

Essa organização permite que o programa seja literalmente idêntico entre todas as implementações (single, multithread e multiprocesso), o que diminui as redundâncias de código e comparações desiguais de tempo de execução.


## Pré-requisitos

É necessário possuir as bibliotecas `pthread` e `freeimage` para compilar o projeto.


## Como utilizar

O programa receberá o caminho da imagem de entrada como primeiro argumento e, como segundo, o caminho da imagem de saída (com efeito aplicado).

Para fazer isso, é necessário compilar o projeto utilizando:
```
make
```

e executamos qualquer um dos comandos abaixo:
```
./out/single_thread (imagem de entrada) (imagem de saída)
./out/multi_thread (imagem de entrada) (imagem de saída)
./out/multi_processo (imagem de entrada) (imagem de saída)
```

Para limpar o ambiente, utilizamos:
```
make clean
```


### Exemplo

Dada imagem de testes abaixo,

![Imagem antes de ser processada](/tests/cachorro.jpg)

após executar `./out/(programa) tests/cachorro.jpg tests/cachorro_out.jpg`, obtemos:

![Imagem após ser processada](/tests/cachorro_out.jpg)
