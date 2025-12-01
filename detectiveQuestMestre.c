#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 101   // tamanho da tabela hash (primo simples)

/* -------------------------
   Structs principais
   -------------------------*/

// Nó da árvore binária que representa uma sala
typedef struct sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];     // pista associada à sala (pode ser string vazia)
    struct sala *esquerda;
    struct sala *direita;
} Sala;

// Nó da BST que guarda as pistas coletadas (ordenadas)
typedef struct pistaNode {
    char pista[MAX_PISTA];
    struct pistaNode *esquerda;
    struct pistaNode *direita;
} PistaNode;

// Nó para tabela hash (encadeamento separado)
typedef struct hashNode {
    char chave[MAX_PISTA];     // a pista (chave)
    char suspeito[MAX_NOME];   // valor: nome do suspeito
    struct hashNode *next;
} HashNode;

/* -------------------------
   Assinaturas das funções
   -------------------------*/

// criarSala() – cria dinamicamente um cômodo.
Sala* criarSala(const char *nome, const char *pista);

// explorarSalas() – navega pela árvore e ativa o sistema de pistas.
void explorarSalas(Sala *raiz, PistaNode **bstPistas, HashNode *hash[]);

// inserirPista() / adicionarPista() – insere a pista coletada na árvore de pistas (BST).
PistaNode* inserirPista(PistaNode *raiz, const char *pista);

// inserirNaHash() – insere associação pista/suspeito na tabela hash.
void inserirNaHash(HashNode *hash[], const char *pista, const char *suspeito);

// encontrarSuspeito() – consulta o suspeito correspondente a uma pista.
const char* encontrarSuspeito(HashNode *hash[], const char *pista);

// exibirPistas() – imprime as pistas coletadas em ordem alfabética.
void exibirPistas(PistaNode *raiz);

// verificarSuspeitoFinal() – conduz à fase de julgamento final.
void verificarSuspeitoFinal(PistaNode *pistasColetadas, HashNode *hash[]);

// funções utilitárias
unsigned long hashString(const char *str);
void inicializarHash(HashNode *hash[]);
void liberarBST(PistaNode *raiz);
void liberarHash(HashNode *hash[]);
void toLowerStr(char *s);

/* -------------------------
   Implementação
   -------------------------*/

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        fprintf(stderr, "Erro: falha na alocação de memória para sala.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(nova->nome, nome, MAX_NOME-1);
    nova->nome[MAX_NOME-1] = '\0';

    if (pista != NULL) {
        strncpy(nova->pista, pista, MAX_PISTA-1);
        nova->pista[MAX_PISTA-1] = '\0';
    } else {
        nova->pista[0] = '\0';
    }
    nova->esquerda = nova->direita = NULL;
    return nova;
}

/* BST: insere pista se não existir; retorna raiz (inserção recursiva)
   Não insere duplicatas (comparação por strcmp). */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || pista[0] == '\0') return raiz;

    if (raiz == NULL) {
        PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
        if (!n) { fprintf(stderr, "Erro: malloc pista\n"); exit(EXIT_FAILURE); }
        strncpy(n->pista, pista, MAX_PISTA-1);
        n->pista[MAX_PISTA-1] = '\0';
        n->esquerda = n->direita = NULL;
        return n;
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (cmp > 0) raiz->direita = inserirPista(raiz->direita, pista);
    // se cmp == 0, já coletada -> não insere duplicata
    return raiz;
}

/* Hash: djb2 */
unsigned long hashString(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % HASH_SIZE;
}

void inicializarHash(HashNode *hash[]) {
    for (int i = 0; i < HASH_SIZE; ++i) hash[i] = NULL;
}

void inserirNaHash(HashNode *hash[], const char *pista, const char *suspeito) {
    if (!pista || pista[0] == '\0') return;
    unsigned long h = hashString(pista);
    // evita inserir duplicata de chave
    for (HashNode *it = hash[h]; it != NULL; it = it->next) {
        if (strcmp(it->chave, pista) == 0) {
            // atualiza suspeito (não deve ocorrer em nosso uso, mas deixamos seguro)
            strncpy(it->suspeito, suspeito, MAX_NOME-1);
            it->suspeito[MAX_NOME-1] = '\0';
            return;
        }
    }
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) { fprintf(stderr, "Erro: malloc hash\n"); exit(EXIT_FAILURE); }
    strncpy(novo->chave, pista, MAX_PISTA-1);
    novo->chave[MAX_PISTA-1] = '\0';
    strncpy(novo->suspeito, suspeito, MAX_NOME-1);
    novo->suspeito[MAX_NOME-1] = '\0';
    novo->next = hash[h];
    hash[h] = novo;
}

const char* encontrarSuspeito(HashNode *hash[], const char *pista) {
    if (!pista || pista[0] == '\0') return NULL;
    unsigned long h = hashString(pista);
    for (HashNode *it = hash[h]; it != NULL; it = it->next) {
        if (strcmp(it->chave, pista) == 0) return it->suspeito;
    }
    return NULL;
}

/* Percorre a BST em ordem e imprime as pistas */
void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/* Conta quantas pistas em 'raiz' (BST) correspondem ao suspeito 'nomeSuspeito'
   Usa a tabela hash para mapear pista -> suspeito. */
int contarPistasPorSuspeitoRec(PistaNode *raiz, HashNode *hash[], const char *nomeSuspeito) {
    if (!raiz) return 0;
    int count = 0;
    count += contarPistasPorSuspeitoRec(raiz->esquerda, hash, nomeSuspeito);
    const char *s = encontrarSuspeito(hash, raiz->pista);
    if (s != NULL && strcmp(s, nomeSuspeito) == 0) count++;
    count += contarPistasPorSuspeitoRec(raiz->direita, hash, nomeSuspeito);
    return count;
}

/* verificarSuspeitoFinal() – conduz à fase de julgamento final.
   Solicita ao jogador o nome do suspeito e verifica se existem ao menos 2 pistas
   que apontem para ele. */
void verificarSuspeitoFinal(PistaNode *pistasColetadas, HashNode *hash[]) {
    if (pistasColetadas == NULL) {
        printf("\nNenhuma pista coletada. Não há base para acusação.\n");
        return;
    }

    printf("\n========================================\n");
    printf("Relatório de pistas coletadas (A → Z):\n");
    exibirPistas(pistasColetadas);
    printf("========================================\n");

    char acusado[MAX_NOME];
    printf("\nDigite o nome do suspeito que deseja acusar (exato): ");
    // limpar buffer e ler linha
    getchar(); // remove o '\n' deixado pelo menu anterior (seguro o uso aqui)
    if (!fgets(acusado, sizeof(acusado), stdin)) {
        printf("Entrada inválida.\n");
        return;
    }
    // remove newline
    acusado[strcspn(acusado, "\r\n")] = '\0';

    if (strlen(acusado) == 0) {
        printf("Nenhum nome digitado. Acusação cancelada.\n");
        return;
    }

    int total = contarPistasPorSuspeitoRec(pistasColetadas, hash, acusado);
    printf("\nO jogo encontrou %d pista(s) que conectam ao suspeito \"%s\".\n", total, acusado);
    if (total >= 2) {
        printf("Resultado: Acusação SUSTENTADA. \"%s\" é considerado culpado com base nas pistas.\n", acusado);
    } else {
        printf("Resultado: Acusação NÃO SUSTENTADA. Não há pistas suficientes para vincular \"%s\" ao crime.\n", acusado);
    }
}

/* explorarSalas() – navega pela árvore e ativa o sistema de pistas.
   Ao visitar uma sala com pista, ela é coletada automaticamente (inserida na BST).
   Para evitar coletar a mesma pista várias vezes, após coletá-la, a pista da sala é
   zerada (pista coletada apenas uma vez por sala). */
void explorarSalas(Sala *atual, PistaNode **bstPistas, HashNode *hash[]) {
    if (!atual) return;
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            // coleciona: inserir na BST (sem duplicatas)
            *bstPistas = inserirPista(*bstPistas, atual->pista);
            // marca como coletada para evitar coleta repetida numa futura visita
            // (simplificação de jogo)
            atual->pista[0] = '\0';
        } else {
            printf("Sem pistas neste cômodo.\n");
        }

        printf("Para onde deseja ir?\n");
        printf(" (e) esquerda\n");
        printf(" (d) direita\n");
        printf(" (s) sair da exploração\n");
        printf("Escolha: ");
        if (scanf(" %c", &escolha) != 1) {
            printf("Entrada inválida. Encerrando exploração.\n");
            return;
        }

        if (escolha == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            return;
        } else if (escolha == 'e') {
            if (atual->esquerda != NULL) atual = atual->esquerda;
            else printf("Não há caminho à esquerda.\n");
        } else if (escolha == 'd') {
            if (atual->direita != NULL) atual = atual->direita;
            else printf("Não há caminho à direita.\n");
        } else {
            printf("Opção inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

/* utilitárias de limpeza de memória */
void liberarBST(PistaNode *raiz) {
    if (!raiz) return;
    liberarBST(raiz->esquerda);
    liberarBST(raiz->direita);
    free(raiz);
}

void liberarHash(HashNode *hash[]) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode *it = hash[i];
        while (it) {
            HashNode *prox = it->next;
            free(it);
            it = prox;
        }
        hash[i] = NULL;
    }
}

/* -------------------------
   main: monta mansão, tabela hash e inicia o jogo
   -------------------------*/
int main(void) {
    /* --- Criação manual do mapa da mansão (árvore binária) --- */
    Sala *hall = criarSala("Hall de Entrada", "Pegadas enluvadas no carpete");
    Sala *salaEstar = criarSala("Sala de Estar", "Perfume caro com resíduo de pó");
    Sala *cozinha = criarSala("Cozinha", "Faca faltando no jogo de talheres");
    Sala *biblioteca = criarSala("Biblioteca", "Página arrancada de um testamento");
    Sala *jardim = criarSala("Jardim Interno", "Terra fresca com impressão de sapato pequeno");
    Sala *despensa = criarSala("Despensa", "Rótulo de veneno rasgado");
    Sala *areaServico = criarSala("Área de Serviço", "Toalha com manchas de óleo");
    Sala *sotao = criarSala("Sótão", "Peça de roupa com bordado inicial 'M.'");

    // montar ligações
    hall->esquerda = salaEstar;
    hall->direita  = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita  = jardim;

    cozinha->esquerda = despensa;
    cozinha->direita  = areaServico;

    biblioteca->esquerda = sotao; // só para criar profundidade (pode ser NULL)

    /* --- Criar tabela hash e popular associação pista -> suspeito --- */
    HashNode *hash[HASH_SIZE];
    inicializarHash(hash);

    // Mapeamentos (definidos estáticamente)
    inserirNaHash(hash, "Pegadas enluvadas no carpete", "Mariana");
    inserirNaHash(hash, "Perfume caro com resíduo de pó", "Roberto");
    inserirNaHash(hash, "Faca faltando no jogo de talheres", "Carlos");
    inserirNaHash(hash, "Página arrancada de um testamento", "Mariana");
    inserirNaHash(hash, "Terra fresca com impressão de sapato pequeno", "Ana");
    inserirNaHash(hash, "Rótulo de veneno rasgado", "Carlos");
    inserirNaHash(hash, "Toalha com manchas de óleo", "Roberto");
    inserirNaHash(hash, "Peça de roupa com bordado inicial 'M.'", "Mariana");

    /* --- BST de pistas coletadas (começa vazia) --- */
    PistaNode *pistasColetadas = NULL;

    printf("============================================\n");
    printf("   Detective Quest — Investigação Final\n");
    printf("============================================\n");
    printf("Você começa no Hall de Entrada. Explore a mansão e colete pistas.\n");

    /* --- Exploração interativa (coleta automática de pistas nas salas visitadas) --- */
    explorarSalas(hall, &pistasColetadas, hash);

    /* --- Fase final: mostrar pistas coletadas e pedir acusação --- */
    verificarSuspeitoFinal(pistasColetadas, hash);

    /* --- Limpeza de memória --- */
    liberarBST(pistasColetadas);
    liberarHash(hash);
    // liberar salas (árvore de salas) - liberação simples em pós-ordem recursiva
    // função de liberação das salas:
    // (implementei inline para manter código compacto)
    // usar pilha/recursão simples:
    // liberar sala tree
    // cria pequena função lambda-like:
    {
        // função local: liberar salas recursivamente
        void liberarSalasRec(Sala *r) {
            if (!r) return;
            liberarSalasRec(r->esquerda);
            liberarSalasRec(r->direita);
            free(r);
        }
        liberarSalasRec(hall);
    }

    printf("\nObrigado por jogar. Fim da simulação.\n");
    return 0;
}
