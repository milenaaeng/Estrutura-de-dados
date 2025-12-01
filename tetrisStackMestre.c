#include <stdio.h>
#include <stdlib.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ------------------------------------------------------------
// Estrutura da peça
// ------------------------------------------------------------
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;
} Peca;

// ------------------------------------------------------------
// Fila circular
// ------------------------------------------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ------------------------------------------------------------
// Pilha
// ------------------------------------------------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ------------------------------------------------------------
int contadorID = 0;

// Prototipos
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

void trocarTopo(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);

void exibirEstado(Fila *f, Pilha *p);

// ------------------------------------------------------------
// MAIN
// ------------------------------------------------------------
int main() {
    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicialmente
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(&fila, gerarPeca());

    int opc;

    do {
        printf("\n===========================================================\n");
        printf("                    ESTADO ATUAL DO JOGO\n");
        printf("===========================================================\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes disponíveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Troca multipla (3 da fila <-> 3 da pilha)\n");
        printf("0 - Sair\n");

        printf("\nOpcao: ");
        scanf("%d", &opc);

        switch(opc) {

        case 1: { // Jogar peça
            if (!filaVazia(&fila)) {
                Peca jogada = dequeue(&fila);
                printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca()); // repõe a fila
            }
            break;
        }

        case 2: { // Reservar
            if (pilhaCheia(&pilha)) {
                printf("\nPilha cheia! Nao e possivel reservar.\n");
            } else if (filaVazia(&fila)) {
                printf("\nFila vazia! Nao ha peca para reservar.\n");
            } else {
                Peca px = dequeue(&fila);
                push(&pilha, px);
                printf("\nPeca [%c %d] movida para a reserva.\n", px.nome, px.id);
                enqueue(&fila, gerarPeca());
            }
            break;
        }

        case 3: { // Usar peça reservada
            if (pilhaVazia(&pilha)) {
                printf("\nPilha vazia! Nenhuma peca para usar.\n");
            } else {
                Peca usada = pop(&pilha);
                printf("\nPeca usada: [%c %d]\n", usada.nome, usada.id);
            }
            break;
        }

        case 4: { // Troca topo <-> fila
            trocarTopo(&fila, &pilha);
            break;
        }

        case 5: { // Troca múltipla
            trocaMultipla(&fila, &pilha);
            break;
        }

        case 0:
            printf("\nEncerrando...\n");
            break;

        default:
            printf("\nOpcao invalida!\n");
            break;
        }

    } while (opc != 0);

    return 0;
}

// ------------------------------------------------------------
// Inicialização
// ------------------------------------------------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// ------------------------------------------------------------
// Verificações
// ------------------------------------------------------------
int filaVazia(Fila *f) { return f->quantidade == 0; }
int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

// ------------------------------------------------------------
// Geração de peça
// ------------------------------------------------------------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// ------------------------------------------------------------
// Operações da fila circular
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// Operações da pilha
// ------------------------------------------------------------
void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = x;
}

Peca pop(Pilha *p) {
    return p->itens[p->topo--];
}

// ------------------------------------------------------------
// Troca simples: topo da pilha ↔ frente da fila
// ------------------------------------------------------------
void trocarTopo(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Nao ha como trocar.\n");
        return;
    }

    if (filaVazia(f)) {
        printf("\nFila vazia! Nao ha como trocar.\n");
        return;
    }

    int idx = f->inicio;

    Peca temp = f->itens[idx];
    f->itens[idx] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
}

// ------------------------------------------------------------
// Troca múltipla: 3 da fila ↔ 3 da pilha
// ------------------------------------------------------------
void trocaMultipla(Fila *f, Pilha *p) {
    if (p->topo < 2 || f->quantidade < 3) {
        printf("\nNao ha 3 pecas em ambas as estruturas. Troca impossivel.\n");
        return;
    }

    int idx = f->inicio;

    for (int i = 0; i < 3; i++) {
        Peca temp = f->itens[(idx + i) % TAM_FILA];
        f->itens[(idx + i) % TAM_FILA] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\nTroca multipla realizada (3 da fila <-> 3 da pilha).\n");
}

// ------------------------------------------------------------
// Exibir o estado
// ------------------------------------------------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\nFila de pecas:\n");
    int idx = f->inicio;

    for (int i = 0; i < f->quantidade; i++) {
        Peca px = f->itens[(idx + i) % TAM_FILA];
        printf("[%c %d] ", px.nome, px.id);
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
