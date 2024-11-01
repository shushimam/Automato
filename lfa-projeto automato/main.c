#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "automato.h"


int main() {
    int flag = 0;
    Estado *estados = NULL;
    Alfabeto *alfabeto = NULL;
    int opcao;
    char arquivo[50];

    while (1) {
        printf("Menu:\n");
        printf("1:leitura de arquivo\n");
        printf("2: verificar a palavra\n");
        printf("3: sair\n");
        printf("Selecione uma opção: ");
        scanf("%d%*c", &opcao);

        
        switch (opcao) {
            case 1:
                 printf(">Insira o nome do arquivo .txt:");
                 scanf("%[^\n]%*c", arquivo);
                flag = leituraArquivo(arquivo,&estados, &alfabeto);
                if (!flag) {
                    printf("Erro na leitura do arquivo...\n");
                } else {
                    printf("Arquivo lido com sucesso!\n");
                }
                break;
            case 2:
                verificaPalavra(estados, alfabeto);
                break;
            case 3:
                printf("Opção 3 selecionada - sair\n");
                return 0;
            default:
                printf("Opção inválida\n");
                break;
        }
    }

    return 0;
}

