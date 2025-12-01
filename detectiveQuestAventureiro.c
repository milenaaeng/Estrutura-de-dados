#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================
    Estrutura da sala da mansão (árvore binária)
===========================================================*/
typedef struct sala {
    char nome[50];
    char pista[100];           // pista opcional do cômodo
    struct sala *esquerda;
    struct sala *direita;
} Sala;

/*===========================================================
    Estrutura do nó da BST de pistas coletadas
===========================================================*/
typedef struct pistaNode {
    char pista[100];
    struct pistaNode *esquerda;
    struct pistaNode *direita;
} PistaNode;

/*===========================================================
    Função criarSala()
    Cria dinamicamente um cômodo com seu nome e pista.
===========================================================*/
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0';  // sem pista

    nova->esquerda = NULL;
    nova->direita  = NULL;
    return nova;
}

/*===========================================================
    inserirPista()
    Insere uma pista na BST de forma ordenada.
===========================================================*/
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita  = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

/*===========================================================
    exibirPistas()
    Imprime a BST em ordem alfabética (in-order).
===========================================================*/
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/*===========================================================
    explorarSalasComPistas()
    Controla a exploração da mansão.
    Coleta automaticamente a pista do cômodo visitado.
===========================================================*/
void explorarSalasComPistas(Sala *atual, PistaNode **pistasColetadas) {
    char escolha;

    while (1) {
        printf("\n📍 Você está em: %s\n", atual->nome);

        // Se houver pista, coleta automaticamente
        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: \"%s\"\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        } else {
            printf("Nenhuma pista disponível neste cômodo.\n");
        }

        // Menu de navegação
        printf("\nPara onde deseja ir?\n");
        printf("(e) Caminho à esquerda\n");
        printf("(d) Caminho à direita\n");
        printf("(s) Sair da exploração\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 's') {
            printf("\n🛑 Exploração encerrada.\n");
            return;
        }
        else if (escolha == 'e') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("🚫 Não existe caminho à esquerda.\n");
        }
        else if (escolha == 'd') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("🚫 Não existe caminho à direita.\n");
        }
        else {
            printf("❗ Opção inválida! Tente novamente.\n");
        }
    }
}

/*===========================================================
    main()
    Cria a árvore da mansão e inicia a exploração.
===========================================================*/
int main() {

    /*===========================================
        Construção manual da estrutura da mansão
    ===========================================*/

    Sala *hall          = criarSala("Hall de Entrada",          "Pegadas suspeitas no carpete");
    Sala *salaEstar     = criarSala("Sala de Estar",            "Perfume forte no ar");
    Sala *cozinha       = criarSala("Cozinha",                  "Faca fora do lugar");
    Sala *biblioteca    = criarSala("Biblioteca",               "Página rasgada de um diário");
    Sala *jardim        = criarSala("Jardim Interno",           "Terra recém-revirada");
    Sala *despensa      = criarSala("Despensa",                 NULL);
    Sala *areaServico   = criarSala("Área de Serviço",          "Roupas molhadas sem explicação");

    hall->esquerda = salaEstar;
    hall->direita  = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita  = jardim;

    cozinha->esquerda = despensa;
    cozinha->direita  = areaServico;


    /*===========================================
        Inicia árvore de pistas coletadas (BST)
    ===========================================*/
    PistaNode *pistasColetadas = NULL;

    printf("===============================================\n");
    printf("    🔍 Detective Quest – Coleta de Pistas\n");
    printf("===============================================\n");

    explorarSalasComPistas(hall, &pistasColetadas);

    /*===========================================
        Exibe todas as pistas coletadas
    ===========================================*/
    printf("\n===============================================\n");
    printf("     📜 Pistas coletadas (A → Z)\n");
    printf("===============================================\n");

    if (pistasColetadas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(pistasColetadas);

    return 0;
}
