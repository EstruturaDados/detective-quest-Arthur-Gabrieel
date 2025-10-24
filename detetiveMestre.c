#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 101

typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[MAX_PISTA];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

typedef struct HashNode {
    char pista[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashNode *proximo;
} HashNode;

typedef struct HashTable {
    HashNode *buckets[HASH_SIZE];
} HashTable;

unsigned long hash_string(const char *str) {
    unsigned long hash = 5381; int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

void trim_inplace(char *s) {
    if (!s) return;
    int n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || isspace((unsigned char)s[n-1]))) n--;
    s[n] = '\0';
    int i = 0; while (s[i] && isspace((unsigned char)s[i])) i++;
    if (i > 0) memmove(s, s+i, strlen(s+i)+1);
}

void str_to_lower(const char *src, char *dst, size_t n) {
    size_t i; for (i=0;i<n-1 && src[i];i++) dst[i]=tolower((unsigned char)src[i]);
    dst[i]='\0';
}

Sala* criarSala(const char *nome, const char *pista) {
    Sala *s=(Sala*)malloc(sizeof(Sala));
    strncpy(s->nome,nome,MAX_NOME-1); s->nome[MAX_NOME-1]='\0';
    if(pista) strncpy(s->pista,pista,MAX_PISTA-1); else s->pista[0]='\0';
    s->esquerda=s->direita=NULL; return s;
}

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if(!pista||pista[0]=='\0') return raiz;
    if(!raiz){
        PistaNode *n=(PistaNode*)malloc(sizeof(PistaNode));
        strncpy(n->pista,pista,MAX_PISTA-1); n->pista[MAX_PISTA-1]='\0';
        n->esquerda=n->direita=NULL; return n;
    }
    int cmp=strcmp(pista,raiz->pista);
    if(cmp<0) raiz->esquerda=inserirPista(raiz->esquerda,pista);
    else if(cmp>0) raiz->direita=inserirPista(raiz->direita,pista);
    return raiz;
}

void inserirNaHash(HashTable *ht,const char *pista,const char *suspeito){
    unsigned long idx=hash_string(pista);
    HashNode *cur=ht->buckets[idx];
    while(cur){ if(strcmp(cur->pista,pista)==0){ strncpy(cur->suspeito,suspeito,MAX_NOME-1); return;} cur=cur->proximo; }
    HashNode *n=(HashNode*)malloc(sizeof(HashNode));
    strncpy(n->pista,pista,MAX_PISTA-1); strncpy(n->suspeito,suspeito,MAX_NOME-1);
    n->proximo=ht->buckets[idx]; ht->buckets[idx]=n;
}

const char* encontrarSuspeito(HashTable *ht,const char *pista){
    unsigned long idx=hash_string(pista); HashNode *cur=ht->buckets[idx];
    while(cur){ if(strcmp(cur->pista,pista)==0) return cur->suspeito; cur=cur->proximo; }
    return NULL;
}

void explorarSalas(Sala *atual, PistaNode **arvorePistas, HashTable *ht){
    char opcao;
    while(atual){
        printf("\nVocê está em: %s\n",atual->nome);
        if(strlen(atual->pista)>0){
            const char *suspeito=encontrarSuspeito(ht,atual->pista);
            printf("Pista encontrada: \"%s\" (Suspeito: %s)\n",atual->pista,suspeito?suspeito:"Desconhecido");
            *arvorePistas=inserirPista(*arvorePistas,atual->pista);
            atual->pista[0]='\0';
        }else printf("Nenhuma pista aqui.\n");

        printf("Deseja ir para (e) esquerda, (d) direita ou (s) sair? ");
        if(scanf(" %c",&opcao)!=1){ while(getchar()!='\n'); continue;}
        if(opcao=='e'||opcao=='E'){ if(atual->esquerda) atual=atual->esquerda; else printf("Não há caminho à esquerda!\n"); }
        else if(opcao=='d'||opcao=='D'){ if(atual->direita) atual=atual->direita; else printf("Não há caminho à direita!\n"); }
        else if(opcao=='s'||opcao=='S'){ printf("Exploração encerrada.\n"); break; }
        else printf("Opção inválida!\n");
    }
}

void exibirPistas(PistaNode *raiz){ if(!raiz) return; exibirPistas(raiz->esquerda); printf("- %s\n",raiz->pista); exibirPistas(raiz->direita); }

int contarPistasParaSuspeito(PistaNode *raiz, HashTable *ht, const char *suspeitoBusca){
    if(!raiz||!ht||!suspeitoBusca) return 0;
    int count=0;
    char alvo[MAX_NOME]; str_to_lower(suspeitoBusca,alvo,sizeof(alvo));
    if(raiz->esquerda) count+=contarPistasParaSuspeito(raiz->esquerda,ht,suspeitoBusca);
    const char *suspeito=encontrarSuspeito(ht,raiz->pista);
    if(suspeito){ char smin[MAX_NOME]; str_to_lower(suspeito,smin,sizeof(smin)); if(strcmp(smin,alvo)==0) count++; }
    if(raiz->direita) count+=contarPistasParaSuspeito(raiz->direita,ht,suspeitoBusca);
    return count;
}

void verificarSuspeitoFinal(PistaNode *arvorePistas, HashTable *ht){
    char entrada[MAX_NOME];
    printf("\nQuem você acusa como culpado? ");
    while(getchar()!='\n'); 
    if(!fgets(entrada,sizeof(entrada),stdin)) return;
    trim_inplace(entrada);
    if(entrada[0]=='\0'){ printf("Nenhum nome informado.\n"); return; }

    int cont=contarPistasParaSuspeito(arvorePistas,ht,entrada);
    printf("\nVocê acusou: %s\n",entrada);
    printf("Pistas que apontam para %s: %d\n",entrada,cont);
    if(cont>=2) printf("✅ Acusação procedente! Você venceu!\n");
    else printf("❌ Evidências insuficientes. Acusação não procedente.\n");
}

void liberarPistas(PistaNode *raiz){ if(!raiz) return; liberarPistas(raiz->esquerda); liberarPistas(raiz->direita); free(raiz); }
void liberarSalas(Sala *raiz){ if(!raiz) return; liberarSalas(raiz->esquerda); liberarSalas(raiz->direita); free(raiz); }
void liberarHash(HashTable *ht){ for(int i=0;i<HASH_SIZE;i++){ HashNode *cur=ht->buckets[i]; while(cur){ HashNode *tmp=cur; cur=cur->proximo; free(tmp);} ht->buckets[i]=NULL; } }

int main(){
    Sala *hall=criarSala("Hall de Entrada","Uma luva caída no chão");
    Sala *salaEstar=criarSala("Sala de Estar","Um copo quebrado sobre a mesa");
    Sala *cozinha=criarSala("Cozinha","Pegadas de lama perto da pia");
    Sala *biblioteca=criarSala("Biblioteca","Um livro aberto sobre crimes antigos");
    Sala *jardim=criarSala("Jardim","Folhas amassadas próximas ao portão");
    Sala *porao=criarSala("Porão","Um cofre trancado com manchas de ferrugem");
    Sala *quarto=criarSala("Quarto","Um lenço com perfume floral");
    Sala *escritorio=criarSala("Escritório","Uma assinatura rasurada em um recibo");

    hall->esquerda=salaEstar; hall->direita=cozinha;
    salaEstar->esquerda=biblioteca; salaEstar->direita=jardim;
    cozinha->direita=porao;
    biblioteca->esquerda=quarto;
    porao->esquerda=escritorio;

    PistaNode *arvorePistas=NULL;
    HashTable ht; for(int i=0;i<HASH_SIZE;i++) ht.buckets[i]=NULL;

    inserirNaHash(&ht,"Uma luva caída no chão","Sr. Almeida");
    inserirNaHash(&ht,"Um copo quebrado sobre a mesa","Sra. Beatriz");
    inserirNaHash(&ht,"Pegadas de lama perto da pia","Carlos");
    inserirNaHash(&ht,"Um livro aberto sobre crimes antigos","Sra. Beatriz");
    inserirNaHash(&ht,"Folhas amassadas próximas ao portão","Carlos");
    inserirNaHash(&ht,"Um cofre trancado com manchas de ferrugem","Sr. Almeida");
    inserirNaHash(&ht,"Um lenço com perfume floral","Dona Teresa");
    inserirNaHash(&ht,"Uma assinatura rasurada em um recibo","Sr. Almeida");

    printf("=== DETECTIVE QUEST (MODO MESTRE) ===\nExplore a mansão e colete pistas.\n");

    explorarSalas(hall,&arvorePistas,&ht);

    printf("\n===== PISTAS COLETADAS =====\n");
    if(arvorePistas==NULL) printf("Nenhuma pista coletada.\n"); else exibirPistas(arvorePistas);

    verificarSuspeitoFinal(arvorePistas,&ht);

    liberarPistas(arvorePistas); liberarSalas(hall); liberarHash(&ht);

    printf("\nFim do jogo.\n");
    return 0;
}
