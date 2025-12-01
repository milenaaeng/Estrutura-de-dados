#include <stdio.h>
#include <stdlib.h>

#define TAM_FILA 5      // Tamanho fixo da fila circular
#define TAM_PILHA 3     // Capacidade máxima da pilha

// -----------------------------------------------------------
// Estrutura da peça
// -----------------------------------------------------------
typedef struct {
    char nome;  // Tipo: I, O, T, L
    int id;     // Identificador único
} Peca;

// -----------------------------------------------------------
// Estrutura da fila circular
// -----------------------------------------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// -----------------------------------------------------------
// Estrutura da pilha linear
// -----------------------------------------------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;   // Índice do topo (último elemento inserido)
} Pilha;

// -----------------------------------------------------------
// Protótipos das funções
// -----------------------------------------------------------
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);

int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);

Peca gerarPeca();
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);

void push(Pilha *p, Peca x);
Peca pop(Pilha *p);

void exibirEstado(Fila *f, Pilha *p);

// ID único global
int contadorID = 0;

// -----------------------------------------------------------
// Função principal
// -----------------------------------------------------------
int main() {
    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicialmente
    for(int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;

    do {
        printf("\n=====================================\n");
        printf("      ESTADO ATUAL DO JOGO\n");
        printf("=====================================\n");

        exibirEstado(&fila, &pilha);

        printf("\nOpcoes disponiveis:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca (fila -> pilha)\n");
        printf("3 - Usar peca reservada (pop)\n");
        printf("0 - Sair\n");

        printf("\nOpcao: ");
        scanf("%d", &opcao);

        switch(opcao) {

        case 1: // Jogar peça
            if (!filaVazia(&fila)) {
                Peca removida = dequeue(&fila);
                printf("\nPeca jogada: [%c %d]\n", removida.nome, removida.id);
                enqueue(&fila, gerarPeca()); // repõe a fila
            } else {
                printf("\nFila vazia! Impossivel jogar.\n");
            }
            break;

        case 2: // Reservar peça (fila → pilha)
            if (filaVazia(&fila)) {
                printf("\nFila vazia! Nao ha peca para reservar.\n");
            } else if (pilhaCheia(&pilha)) {
                printf("\nPilha cheia! Nao e possivel reservar.\n");
            } else {
                Peca reservada = dequeue(&fila);
                push(&pilha, reservada);
                printf("\nPeca [%c %d] reservada.\n", reservada.nome, reservada.id);
                enqueue(&fila, gerarPeca()); // mantém fila cheia
            }
            break;

        case 3: // Usar peça reservada
            if (pilhaVazia(&pilha)) {
                printf("\nPilha vazia! Nenhuma peca para usar.\n");
            } else {
                Peca usada = pop(&pilha);
                printf("\nPeca reservada usada: [%c %d]\n", usada.nome, usada.id);
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

// -----------------------------------------------------------
// Inicialização
// -----------------------------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// -----------------------------------------------------------
// Verificações
// -----------------------------------------------------------
int filaVazia(Fila *f) { return f->quantidade == 0; }
int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

// -----------------------------------------------------------
// Função que gera uma peça aleatória
// -----------------------------------------------------------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;  
    return p;
}

// -----------------------------------------------------------
// Operações da fila circular
// -----------------------------------------------------------
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

Peca dequeue(Fila *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

// -----------------------------------------------------------
// Operações da pilha
// -----------------------------------------------------------
void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = x;
}

Peca pop(Pilha *p) {
    return p->itens[p->topo--];
}

// -----------------------------------------------------------
// Exibir o estado atual da fila e da pilha
// -----------------------------------------------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\nFila de pecas:\n");

    int i = f->inicio;
    for (int c = 0; c < f->quantidade; c++) {
        Peca px = f->itens[i];
        printf("[%c %d] ", px.nome, px.id);
        i = (i + 1) % TAM_FILA;
    }

    printf("\n\nPilha de reserva (Topo -> Base):\n");

    if (pilhaVazia(p)) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }

    printf("\n");
}
