#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estrutura da Árvore ---
// Representa uma sala (nó) da mansão.
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- Função: criarSala ---
// Cria uma nova sala dinamicamente e retorna seu ponteiro.
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// --- Função: explorarSalas ---
// Permite o jogador navegar pela árvore binária.
void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se não há mais caminhos, o jogador chegou a um nó-folha.
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Você chegou ao final da exploração.\n");
            break;
        }

        printf("Deseja ir para (e) esquerda, (d) direita ou (s) sair? ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há caminho à esquerda!\n");
        } 
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há caminho à direita!\n");
        } 
        else if (opcao == 's' || opcao == 'S') {
            printf("Exploração encerrada.\n");
            break;
        } 
        else {
            printf("Opção inválida! Escolha e, d ou s.\n");
        }
    }
}

// --- Função principal (main) ---
// Monta o mapa da mansão e inicia a exploração.
int main() {
    // Criação das salas (árvore fixa e manual)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");

    // Montagem da árvore binária
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    printf("=== DETECTIVE QUEST ===\n");
    printf("Explore a mansão e encontre o culpado!\n");

    // Inicia a exploração a partir do Hall
    explorarSalas(hall);

    // Libera memória (opcional, mas boa prática)
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(porao);

    return 0;
}
