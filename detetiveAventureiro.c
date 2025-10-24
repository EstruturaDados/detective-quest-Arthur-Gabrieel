#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estrutura da árvore de cômodos ---
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- Estrutura da árvore de pistas (BST) ---
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// --- Função: criarSala ---
// Cria dinamicamente uma sala com nome e pista opcional.
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// --- Função: inserirPista ---
// Insere uma nova pista na árvore BST em ordem alfabética.
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *nova = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// --- Função: exibirPistas ---
// Exibe todas as pistas coletadas em ordem alfabética (in-order traversal).
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// --- Função: explorarSalasComPistas ---
// Controla a navegação do jogador e coleta automática das pistas.
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se o cômodo tiver uma pista, coleta automaticamente
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            // Evita coletar a mesma pista novamente
            strcpy(atual->pista, "");
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }

        // Verifica se é um cômodo sem saídas
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou ao fim deste caminho.\n");
        }

        printf("Deseja ir para (e) esquerda, (d) direita ou (s) sair? ");
        scanf(" %c", &opcao);

        // Verifica a opção escolhida
        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda!\n");
            }
        } 
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita!\n");
            }
        } 
        else if (opcao == 's' || opcao == 'S') {
            printf("\nExploração encerrada pelo jogador.\n");
            break;
        } 
        else {
            printf("Opção inválida! Use e, d ou s.\n");
        }
    }
}

// --- Função: liberarMemoriaSalas ---
// Libera a memória da árvore de salas (recursivamente).
void liberarMemoriaSalas(Sala *raiz) {
    if (raiz != NULL) {
        liberarMemoriaSalas(raiz->esquerda);
        liberarMemoriaSalas(raiz->direita);
        free(raiz);
    }
}

// --- Função: liberarMemoriaPistas ---
// Libera a memória da árvore de pistas (recursivamente).
void liberarMemoriaPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarMemoriaPistas(raiz->esquerda);
        liberarMemoriaPistas(raiz->direita);
        free(raiz);
    }
}

// --- Função principal (main) ---
int main() {
    // Criação do mapa fixo da mansão
    Sala *hall = criarSala("Hall de Entrada", "Uma luva caída no chão");
    Sala *salaEstar = criarSala("Sala de Estar", "Um copo quebrado sobre a mesa");
    Sala *cozinha = criarSala("Cozinha", "Pegadas de lama perto da pia");
    Sala *biblioteca = criarSala("Biblioteca", "Um livro aberto sobre crimes antigos");
    Sala *jardim = criarSala("Jardim", "Folhas amassadas próximas ao portão");
    Sala *porao = criarSala("Porão", "Um cofre trancado com manchas de ferrugem");

    // Montagem manual da árvore de salas
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    // Inicializa árvore de pistas vazia
    PistaNode *arvorePistas = NULL;

    printf("=== DETECTIVE QUEST: COLETA DE PISTAS ===\n");
    printf("Explore a mansão, encontre pistas e resolva o mistério!\n");

    // Inicia exploração
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe pistas coletadas
    printf("\n===== PISTAS COLETADAS =====\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Libera memória
    liberarMemoriaSalas(hall);
    liberarMemoriaPistas(arvorePistas);

    printf("\nFim da investigação. Até a próxima, detetive!\n");
    return 0;
}
