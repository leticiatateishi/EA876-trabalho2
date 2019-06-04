##
# Rafael Sartori M. Santos, 186154
# Letícia Mayumi Araújo Tateishi, 201454
#
# Trabalho 2 de EA876 1s2019 na Unicamp
##

CC = gcc
FLAGS = -Wall -lpthread -lfreeimage

# Pastas
SOURCES = ./src
OUTPUT = ./out

# Macros para teste
BASH = sh
TEST_SCRIPT = test.sh
VERBOSE ?= 1


all: $(OUTPUT)/single_thread $(OUTPUT)/multi_thread $(OUTPUT)/multi_processos

$(OUTPUT)/multi_processos: $(OUTPUT)/processamento.o $(OUTPUT)/imagem.o $(OUTPUT)/main.o $(SOURCES)/multi_processos.c
	$(CC) -o $(OUTPUT)/multi_processos $(FLAGS) $(OUTPUT)/processamento.o $(OUTPUT)/imagem.o $(OUTPUT)/main.o $(SOURCES)/multi_processos.c

$(OUTPUT)/multi_thread: $(OUTPUT)/processamento.o $(OUTPUT)/imagem.o $(OUTPUT)/main.o $(SOURCES)/multi_thread.c
	$(CC) -o $(OUTPUT)/multi_thread $(FLAGS) $(OUTPUT)/processamento.o $(OUTPUT)/imagem.o $(OUTPUT)/main.o $(SOURCES)/multi_thread.c

$(OUTPUT)/single_thread: $(OUTPUT)/processamento.o $(OUTPUT)/imagem.o $(OUTPUT)/main.o $(SOURCES)/single_thread.c
	$(CC) -o $(OUTPUT)/single_thread $(FLAGS) $(OUTPUT)/processamento.o $(OUTPUT)/imagem.o $(OUTPUT)/main.o $(SOURCES)/single_thread.c

$(OUTPUT)/processamento.o: $(SOURCES)/processamento.c $(SOURCES)/processamento.h
	$(CC) -o $(OUTPUT)/processamento.o $(FLAGS) -c $(SOURCES)/processamento.c

$(OUTPUT)/imagem.o: $(SOURCES)/imagem.c $(SOURCES)/imagem.h
	$(CC) -o $(OUTPUT)/imagem.o $(FLAGS) -c $(SOURCES)/imagem.c

$(OUTPUT)/main.o: $(SOURCES)/main.c
	$(CC) -o $(OUTPUT)/main.o $(FLAGS) -c $(SOURCES)/main.c

test: all
	$(BASH) $(TEST_SCRIPT) $(OUTPUT)/main $(VERBOSE)

clean:
	rm $(OUTPUT)/*
