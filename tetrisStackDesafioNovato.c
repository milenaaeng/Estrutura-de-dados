#include <stdio.h>
#include <stdlib.h>

#define TAM_FILA 5   // Tamanho fixo da fila

// ----------------------------
// Definição da peça
// ----------------------------
typedef struct {
    char nome;  // Tipo da peça: I, O, T, L
    int id;     // Identificador único
} Peca;

// ----------------------------
// Fila circular de peças
// ----------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ----------------------------
// Protótipos das funções
// ----------------------------
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
Peca gerarPeca();
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
void exibirFila(Fila *f);

// Variável global simples para IDs únicos
int contadorID = 0;

// ----------------------------
// Função principal
// ----------------------------
int main() {
    Fila fila;
    inicializarFila(&fila);

    // Inicializa fila com 5 peças automáticas
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;

    do {
        printf("\n=============================\n");
        printf("     Fila de Pecas Tetris\n");
        printf("=============================\n");

        exibirFila(&fila);

        printf("\nOpcoes:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            if (!filaVazia(&fila)) {
                Peca jogada = dequeue(&fila);
                printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);
            } else {
                printf("\nA fila esta vazia! nao ha peca pra jogar.\n");
            }
            break;

        case 2:
            if (!filaCheia(&fila)) {
                enqueue(&fila, gerarPeca());
            } else {
                printf("\nA fila esta cheia! nao e possivel inserir.\n");
            }
            break;

        case 0:
            printf("\nEncerrando o programa...\n");
            break;

        default:
            printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// ----------------------------
// Inicializa fila vazia
// ----------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

// ----------------------------
// Verifica se fila está vazia
// ----------------------------
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// ----------------------------
// Verifica se fila está cheia
// ----------------------------
int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

// ----------------------------
// Gerar peça automaticamente
// ----------------------------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;

    p.nome = tipos[rand() % 4];
    p.id = contadorID++;

    return p;
}

// ----------------------------
// Inserção na fila (enqueue)
// ----------------------------
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

// ----------------------------
// Remoção da fila (dequeue)
// ----------------------------
Peca dequeue(Fila *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

// ----------------------------
// Exibição da fila formatada
// ----------------------------
void exibirFila(Fila *f) {
    printf("\nFila de pecas:\n");

    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }

    int i = f->inicio;

    for (int c = 0; c < f->quantidade; c++) {
        Peca p = f->itens[i];
        printf("[%c %d] ", p.nome, p.id);
        i = (i + 1) % TAM_FILA;
    }

    printf("\n");
}
