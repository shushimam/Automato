#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "automato.h"

//-----------------------------------------PILHA--------------------
int vazia(Pilha* p) {
    return (p->topo == NULL);
}

Pilha* criar_pilha_vazia() {
    Pilha* aux = (Pilha*) malloc(sizeof(Pilha));
    aux->topo = NULL;
    return aux;
}


// Empilha um item
void push(Pilha* p, char* x) {
    if(*x=='\0')return;
    else{
    struct no* aux = (struct no*) malloc(sizeof(struct no));
    aux->info = *x;
    aux->prox = p->topo;
    p->topo = aux;
    x++;
    push(p,x);
    }
}

//Desempilha um item
int pop(Pilha* p, char* info) {
    if(!vazia(p)) {
        if(*info==p->topo->info){
            struct no* aux = p->topo;
            p->topo = p->topo->prox;
            free(aux);
        }
        else  return 1;
        info++;
        if(*info == '\0') return 0;
        else return pop(p, info);
    }
    else return 1;
        
}


//----------------------------------------------------------ALFABETO-----------------------------

Alfabeto* insereAlfabeto(Alfabeto* lista, char letra){
    if(lista == NULL){
        lista = (Alfabeto*) malloc(sizeof(Alfabeto));
        lista->letra = letra;
        lista->prox = NULL;
    }
    else
        lista->prox = insereAlfabeto(lista->prox, letra); //Passagem recursiva para o próximo elemento da lista
    return lista;
}

Alfabeto* lerAlfabeto_fita(Alfabeto* lista, FILE* file){
    char aux,aux2[40],aux3;
    fscanf(file, "%[^{]%*c", aux2);//Mascaração da leitura do arquivo
    do{
        
        fscanf(file, "%c%c", &aux,&aux3);
        lista = insereAlfabeto(lista, aux);
    }while(aux3 != '}');    
    fscanf(file,"\n");

    return lista;

}

Alfabeto* lerAlfabeto_pilha(Alfabeto* lista, FILE* file){
    char aux,aux2[40],aux3;
    fscanf(file, "%[^{]%*c", aux2);//Mascaração da leitura do arquivo
    do{
        
        fscanf(file, "%c%c", &aux,&aux3);
        lista = insereAlfabeto(lista, aux);
    }
    while(aux3 != '}');
    fscanf(file,"\n");
    return lista;
}

int verificaAlfabeto(char simbolo, Alfabeto* alfabeto){
    for(; alfabeto != NULL; alfabeto = alfabeto->prox) {//Passagem de nó a nó
        if(simbolo == alfabeto->letra || simbolo == '\0') {  return 1;}
    }
    return 0; //Se não está contido retona 0
}

//----------------------------------------------ESTADOS---------------------------------------

Estado* defineFinais(Estado* lista, FILE* file) {
    int flag = 0;
    Estado* aux;
    int aux1;
    char aux2,aux3[40];
   fscanf(file, "%[^{]%*c",aux3);

    do{
   
    fscanf(file,"%*c%d%c",&aux1,&aux2);
        for(aux = lista; aux!=NULL; aux = aux->prox){
            if(aux->estado == aux1){
                aux->final=1;
                break;
            }
        }
        if (flag)
            break;
    }while (aux2 ==',' );

    return lista;
}

Estado* insereEstado(Estado* lista, int nome){
    if(lista == NULL){ //Verificação se esta no fim da lista
        lista = (Estado*) malloc(sizeof(Estado));
        lista->estado = nome;
        lista->final = 0; //Por padrão, não será considerado como final
        lista->no = NULL;
        lista->prox = NULL;
    }
    else
        lista->prox = insereEstado(lista->prox, nome); //Passagem recursiva para o próximo elemento da lista
    return lista;
}
Estado* LerEstado(Estado* lista, FILE* file) {
    int  i = 0, tamanho;
    char aux3[40];
    fscanf(file, "%[^=]%*c",aux3);
    fscanf(file, "%d%*c", &tamanho);
    while (i < tamanho) {
        lista = insereEstado(lista, i);

        i++;
    }
    return lista;
}
//------------------------------------------------TRANSIÇÃO---------------------------------------------------------------

Transicao* insereTransicao(Transicao* lista, char simbolo, int destino, Estado* estados, char Empilha[], char Desempilha[]){
    Estado* aux;
    if(lista == NULL){
        lista = (Transicao*) malloc(sizeof(Transicao));
        lista->simbolo = simbolo;
        lista->prox = NULL;
        strcpy(lista->empilha, Empilha);
        strcpy(lista->desempilha, Desempilha);

        for(aux = estados; aux->estado!= destino; aux = aux->prox);
        lista->estado = aux;
    }
    else
        lista->prox = insereTransicao(lista->prox, simbolo, destino, estados, Empilha , Desempilha); 
        return lista;
}

Estado* lerTransicao(Estado* lista, FILE* file){
    char simbolo,Empilha[40],Desempilha[40],aux2;
    int Origem,Destino;
    Estado *aux;
    while(fscanf(file,"%c",&aux2) != EOF){ //Como é o começo da discrição de toda transição, sempre é verificado
        fscanf(file, "%*c%d", &Origem);
        fscanf(file,"%*c%c%*c",&simbolo);
        fscanf(file,"%[^)]%*c%*c=%*c(",Desempilha);
        fscanf(file,"%*c%d%*c",&Destino);
        fscanf(file,"%[^)]%*c\n",Empilha);
        for(aux = lista; Origem !=aux->estado; aux = aux->prox); //Vai procurar o estado em que ocorre a transição
             aux->no = insereTransicao(aux->no, simbolo, Destino, lista, Empilha, Desempilha);
        
   
    }
    return lista;
}

Estado* realizaTransicao(char simbolo, Transicao* no, Pilha* p, int* posi){
    Estado* estado = NULL;
    int flag = 1;
    Transicao* aux = no;
    for(; no != NULL; no = no->prox){
        if(simbolo == no->simbolo && (p->topo == NULL || (p->topo->info) == *(no->desempilha) || !strcmp(no->desempilha, "@"))){
            if(strcmp(no->empilha, "@")){
                push(p, no->empilha);
            }
            if(strcmp(no->desempilha, "@")){
                if(pop(p, no->desempilha)) return NULL;
            }
            estado = no->estado;
            (*posi)++;
            flag = 0;
            break;
        }
    }
    for(; aux != NULL && flag; aux = aux->prox){
        if(aux->simbolo == '@'){
            if(strcmp(aux->empilha, "@")){
                push(p, aux->empilha);
            }
            if(strcmp(aux->desempilha, "@")){
                if(pop(p, aux->desempilha)) return NULL;
            }
            estado = aux->estado;
            break;
        }
    }
 
    return estado;
}

//--------------------------------------PALAVRA------------------------------------------

void verificaPalavra(Estado* estados, Alfabeto* alfabeto){
    Pilha *p=criar_pilha_vazia();
    char palavra[101],aux[101];
    int flagAlfa, flagTransi; //flags para caso ocorra alguma condição
    Estado* estadoAtual = estados; //Para começar pelo estado 'q0'
    int i;

    printf(">Escreva a palavra:");
    scanf("%[^\n]%*c", palavra);

    for(i = flagAlfa = flagTransi = 0; palavra[i] != '\0' && palavra[i] != '@' || p->topo != NULL;){

      
        if(!verificaAlfabeto(palavra[i], alfabeto)){ //verifica se o caractere está no alfabeto da linguagem
            flagAlfa = 1;
            break;
        }
            imprimir(palavra,i,estadoAtual->estado,p);
            estadoAtual = realizaTransicao(palavra[i], estadoAtual->no,p,&i);
            
        if(estadoAtual == NULL){ //se estadoAtual for nulo não foi realizada a transição
            flagTransi = 1;
            break;
        }
    }
     if(estadoAtual != NULL) imprimir(palavra, i, estadoAtual->estado, p);
     
    if(flagAlfa || flagTransi || !estadoAtual->final) printf("REJEITA\n");
    else printf("ACEITA\n");
}

//--------------------------------------ARQUIVO--------------------------------------------

int leituraArquivo(char arquivo[],Estado** estados, Alfabeto** alfabeto){
    FILE* finput;
    
    
    finput = fopen(arquivo, "r");

    if(finput == NULL) return 0; //Caso a leitura do arquivo dê errado
    *estados = LerEstado(*estados, finput);

    *estados = defineFinais(*estados, finput);

    *alfabeto = lerAlfabeto_fita(*alfabeto, finput);

    *alfabeto = lerAlfabeto_pilha(*alfabeto, finput);
    lerTransicao(*estados, finput);
    fclose(finput);
    return 1; //Caso a leitura dê certo
}


//----------------------------------------------------print-----------------------------------------------------

void imprimirPilha(struct no* pilha){
    if(pilha == NULL) printf("\n");
    else{
        printf("%c", pilha->info);
        imprimirPilha(pilha->prox);
    }
}

void imprimir(char palavra[], int indiceAtual, int estadoAtual, Pilha* pilha){
    int i;

    printf("fita: ");
    for(i = 0; palavra[i] != '\0' || palavra[i-1] != '\0' ; i++){
        if(i == indiceAtual){
            printf("[q%d]", estadoAtual);
        }
        printf("%c", palavra[i]);
    }
    
    printf(" pilha: ");
    imprimirPilha(pilha->topo);
}



