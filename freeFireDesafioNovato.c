#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// -----------------------------
// DEFINIÇÃO DA STRUCT ITEM
// -----------------------------
typedef struct {
    char nome[30];      // Nome do item
    char tipo[20];      // Tipo do item (arma, munição, cura, etc)
    int quantidade;     // Quantidade do item
} Item;

// Vetor de itens (mochila)
Item mochila[MAX_ITENS];
int totalItens = 0;

// -----------------------------
// FUNÇÃO: LISTAR ITENS
// -----------------------------
void listarItens() {
    printf("\n--- ITENS NA MOCHILA ---\n");
    if (totalItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    for (int i = 0; i < totalItens; i++) {
        printf("Item %d:\n", i + 1);
        printf("  Nome: %s\n", mochila[i].nome);
        printf("  Tipo: %s\n", mochila[i].tipo);
        printf("  Quantidade: %d\n", mochila[i].quantidade);
    }
}

// -----------------------------
// FUNÇÃO: INSERIR ITEM
// -----------------------------
void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\nA mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;

    printf("\nDigite o nome do item: ");
    scanf("%s", novo.nome);

    printf("Digite o tipo do item (arma, municao, cura...): ");
    scanf("%s", novo.tipo);

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    // Armazena no vetor
    mochila[totalItens] = novo;
    totalItens++;

    printf("\nItem cadastrado com sucesso!\n");
    listarItens();
}

// -----------------------------
// FUNÇÃO: BUSCAR ITEM POR NOME (BUSCA SEQUENCIAL)
// -----------------------------
int buscarItem(char nome[]) {
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i; // Encontrou
        }
    }
    return -1; // Não encontrou
}

// -----------------------------
// FUNÇÃO: REMOVER ITEM
// -----------------------------
void removerItem() {
    if (totalItens == 0) {
        printf("\nA mochila está vazia. Nada para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item a remover: ");
    scanf("%s", nomeRemover);

    int index = buscarItem(nomeRemover);

    if (index == -1) {
        printf("\nItem não encontrado.\n");
        return;
    }

    // Remove deslocando os itens para trás
    for (int i = index; i < totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    totalItens--;
    printf("\nItem removido com sucesso!\n");

    listarItens();
}

// -----------------------------
// FUNÇÃO PRINCIPAL (MENU)
// -----------------------------
int main() {
    int opcao;

    do {
        printf("\n==============================\n");
        printf("       MOCHILA DO JOGADOR     \n");
        printf("==============================\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4: {
                char nomeBusca[30];
                printf("\nDigite o nome do item a buscar: ");
                scanf("%s", nomeBusca);

                int pos = buscarItem(nomeBusca);

                if (pos == -1) {
                    printf("\nItem não encontrado.\n");
                } else {
                    printf("\nItem encontrado!\n");
                    printf("Nome: %s\n", mochila[pos].nome);
                    printf("Tipo: %s\n", mochila[pos].tipo);
                    printf("Quantidade: %d\n", mochila[pos].quantidade);
                }
                break;
            }
            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
