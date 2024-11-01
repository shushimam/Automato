#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char* TipoItem;

struct no {
    char info;
    struct no* prox;
};
//---------------------------------------------PILHA--------------------
typedef struct {
    struct no* topo;
}Pilha;

int vazia(Pilha* p);

//cria pilha vazia
Pilha* criar_pilha_vazia();

// Lê o topo da pilha sem desempilhar
TipoItem* topo(Pilha* p);

// Empilha um item
void push(Pilha* p, TipoItem x);

//Desempilha um item
int pop(Pilha* p, TipoItem info);

//imprime a pilha recursivamente
void imprime_rec(struct no* p);

//********************************************************************************************
//--------------------------------------------AUTOMATO------------------------
typedef struct alfa{
    char letra; //A letra do alfabeto
    struct alfa* prox; //proximo elemento

}Alfabeto;

// Estrutura responsável pelas transições possiveis
typedef struct no1{
    char simbolo; //Caractere da passagem
    char empilha[40];
    char desempilha[40];
    struct no2* estado; //Se for o simbolo vai para o endereço do estado
    struct no1* prox; // o próximo elemento da lista de transições

}Transicao;

// Estrutura dos estados
typedef struct no2{
    int estado; // qual terminal se encontra 
    int final; //1: terminal 0: não-terminal
    Transicao* no; //guarda as transições possíveis
    struct no2* prox; //próximo elemento da lista de estados

}Estado;
//********************************************************************************************
//-------------------------------------------FUNC   ALAFABETO---------------------

// Insere o alafabeto recebido na lista
// Entrada: uma lista dos Caracteres á inserir
// Retorno: a lista com os caracteres inseridos
// Pré-condição: nenhuma 
// Pós-condição: armazena os caracteres
Alfabeto* insereAlfabeto(Alfabeto* lista, char letra);
// Lê o Alfabeto do arquivo para a fita
// Entrada: um arquivo padronizada para leitura e uma lista vazia
// Retorno: uma lista com os caracteres lido do arquivo 
// Pré-condição: lista =NULL
// Pós-condição: aramazena os caracteres
Alfabeto* lerAlfabeto_fita(Alfabeto* lista, FILE* file);
// Lê o Alfabeto do aquivo para a pilha
// Entrada: uma lista vazia e um arquivo de texto 
// Retorno: uma lista modificada com os caracteres lidos
// Pré-condição: lista = NULL 
// Pós-condição: aramazena os caracteres
Alfabeto* lerAlfabeto_pilha(Alfabeto* lista, FILE* file);
// verifica o caractere da palavra esta contido no alfabeto
// Entrada: um char que sera analizado e uma   
// Retorno: 1 se o caracter analizado esta no alfabeto e 0 se não
// Pré-condição: uma palavra e um alfabeto ja preenchido 
// Pós-condição: nenhuma
int verificaAlfabeto(char simbolo, Alfabeto* alfabeto);
//********************************************************************************************
//--------------------------------------------ESTADOS---------------------------
// le do arquivo quais serão os estados finais 
// Entrada: uma lista vazia e um arquivo texto
// Retorno: uma lista modificada
// Pré-condição: lista = NULL
// Pós-condição: armazena os valores na lista
Estado* defineFinais(Estado* lista, FILE* file);
// le do arquivo quais serão os estados
// Entrada: uma lista vazia e um arquivo texto
// Retorno: uma lista modificada
// Pré-condição: lista = NULL
// Pós-condição: armazena os valores na lista
Estado* insereEstado(Estado* lista, int nome);
// Lê o Alfabeto do arquivo para a lista
// Entrada: um arquivo padronizada para leitura e uma lista vazia
// Retorno: uma lista com o inteiros lido do arquivo 
// Pré-condição: lista =NULL
// Pós-condição: aramazena os inteiros
Estado* LerEstado(Estado* lista, FILE* file);
//********************************************************************************************
//------------------------------------------TRANSIÇÕES------------------------
// cria um ponteiro apontando para quais estados e qual o simbulo necessaria para realizar uma transição
// Entrada: uma lista de transição á inserir
// Retorno: a lista com os caracteres inseridos
// Pré-condição: nenhuma 
// Pós-condição: armazena as Transicões
Transicao* insereTransicao(Transicao* lista, char simbolo, int destino, Estado* estados, char* Empilha, char *Desempilha);
// Lê as Transicões do arquivo para a lista
// Entrada: um arquivo padronizada para leitura e uma lista vazia
// Retorno: uma lista com as Transicões lidas do arquivo 
// Pré-condição: lista =NULL
// Pós-condição: aramazena as Transicões
Estado* lerTransicao(Estado* lista, FILE* file);
// realiza as Transicao desejadas em um estado auxiliar para a verificação da palavra de acordo com a fita
// Entrada: o char da fita analizada as transições armazenadas, uma pilha vazia e a posição em que está 
// Retorno: o estado para qual transitou
// Pré-condição: uma palavra ja lida, as transições ja definidas
// Pós-condição: altera o estado auxiliar
Estado* realizaTransicao(char simbolo, Transicao* no, Pilha* p, int* posi);
//********************************************************************************************
//-------------------------------------------PALAVRA-------------------------
// desencadeia todas as funções para a verificação de uma palavra alem de imprimir a analise feira na palavra (o estado em que esta e a pilha)
// Entrada: os estados e o alfabeto
// Retorno: nenhum
// Pré-condição: estados e alfabeto ja definidos 
// Pós-condição: nenhuma
void verificaPalavra(Estado* estados, Alfabeto* alfabeto);
//********************************************************************************************
//-------------------------------------------ARQUIVO-------------------------
// le o arquivo
// Entrada: o nome do arquivo, estado e alfabeto
// Retorno: 0 para erro na leitura e 1 para sucesso
// Pré-condição: nome do arquivo, estado e alfabeto ja inicializados 
// Pós-condição: armazena os valores nos suas respectivas structs
int leituraArquivo(char arquivo[],Estado** estados, Alfabeto** alfabeto);
//********************************************************************************************
//---------------------------------------------PRINT---------------------------------------------
// imprime a pilha 
// Entrada: a pilha
// Retorno: nenhum
// Pré-condição: pilha ja inicializada 
// Pós-condição: nenhuma
void imprimirPilha(struct no* pilha);
// imprime a palavra e suas transições 
// Entrada: a palavra a posição e o estado em que se esta analizando e a pilha
// Retorno: nenhum
// Pré-condição: nenhuma
// Pós-condição: nenhuma
void imprimir(char palavra[], int indiceAtual, int estadoAtual, Pilha* pilha);
//********************************************************************************************
