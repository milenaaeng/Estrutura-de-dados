#include <stdio.h>
#include <string.h>

/*
    Definição da struct Territorio
    Agrupa informações relacionadas a cada território do jogo:
    - nome: nome do território
    - cor: cor do exército que ocupa o território
    - tropas: quantidade de tropas posicionadas nele
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {

    // Vetor para armazenar 5 territórios
    Territorio territorios[5];

    printf("===== Cadastro de Territórios =====\n\n");

    // Entrada de dados dos 5 territórios
    for(int i = 0; i < 5; i++) {
        printf("Cadastro do Território %d:\n", i + 1);

        printf("Nome do território: ");
        scanf("%s", territorios[i].nome);

        printf("Cor do exército: ");
        scanf("%s", territorios[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n");
    }

    // Exibição dos territórios cadastrados
    printf("===== Territórios Cadastrados =====\n\n");

    for(int i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
        printf("------------------------------\n");
    }

    return 0;
}