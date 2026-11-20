#include <stdio.h>
#include <stdlib.h>
#include "retrospectiva.h"

/* ------------------------------------------------------------------ */
/* AVL de pares (chave -> contagem).                                   */
/* Usada nas linhas (chave = musica) e nas colunas (chave = usuario).  */
/* Cada no guarda tambem o melhor par da sua subarvore (maior          */
/* contagem; no empate, menor chave), o que da a FAVORITA em O(1).     */
/* ------------------------------------------------------------------ */

typedef struct no_p NOP;
struct no_p {
    int chave;
    int cont;
    int melhor_chave;
    int melhor_cont;
    int altura;
    NOP *esq;
    NOP *dir;
};

static int altura_p(NOP *n) {
    return n == NULL ? 0 : n->altura;
}

static int melhor_que(int cont1, int chave1, int cont2, int chave2) {
    return cont1 > cont2 || (cont1 == cont2 && chave1 < chave2);
}

static void atualiza_p(NOP *n) {
    int ae = altura_p(n->esq);
    int ad = altura_p(n->dir);
    n->altura = 1 + (ae > ad ? ae : ad);

    n->melhor_chave = n->chave;
    n->melhor_cont = n->cont;
    if (n->esq != NULL && melhor_que(n->esq->melhor_cont, n->esq->melhor_chave,
                                     n->melhor_cont, n->melhor_chave)) {
        n->melhor_chave = n->esq->melhor_chave;
        n->melhor_cont = n->esq->melhor_cont;
    }
    if (n->dir != NULL && melhor_que(n->dir->melhor_cont, n->dir->melhor_chave,
                                     n->melhor_cont, n->melhor_chave)) {
        n->melhor_chave = n->dir->melhor_chave;
        n->melhor_cont = n->dir->melhor_cont;
    }
}

static NOP* rot_dir_p(NOP *n) {
    NOP *e = n->esq;
    n->esq = e->dir;
    e->dir = n;
    atualiza_p(n);
    atualiza_p(e);
    return e;
}

static NOP* rot_esq_p(NOP *n) {
    NOP *d = n->dir;
    n->dir = d->esq;
    d->esq = n;
    atualiza_p(n);
    atualiza_p(d);
    return d;
}

static NOP* balanceia_p(NOP *n) {
    atualiza_p(n);
    int fb = altura_p(n->esq) - altura_p(n->dir);

    if (fb > 1) {
        if (altura_p(n->esq->esq) < altura_p(n->esq->dir)) {
            n->esq = rot_esq_p(n->esq);
        }
        return rot_dir_p(n);
    }
    if (fb < -1) {
        if (altura_p(n->dir->dir) < altura_p(n->dir->esq)) {
            n->dir = rot_dir_p(n->dir);
        }
        return rot_esq_p(n);
    }
    return n;
}

/* Insere a chave com contagem 1 ou soma 1 se ela ja existe. *novo = 1 se criou o no. */
static NOP* inserir_p(NOP *r, int chave, int *novo) {
    if (r == NULL) {
        NOP *n = (NOP *) malloc(sizeof(NOP));
        if (n == NULL) {
            *novo = 0;
            return NULL;
        }
        n->chave = chave;
        n->cont = 1;
        n->esq = NULL;
        n->dir = NULL;
        atualiza_p(n);
        *novo = 1;
        return n;
    }
    if (chave < r->chave) {
        r->esq = inserir_p(r->esq, chave, novo);
    } else if (chave > r->chave) {
        r->dir = inserir_p(r->dir, chave, novo);
    } else {
        r->cont++;
        *novo = 0;
    }
    return balanceia_p(r);
}

static NOP* remover_p(NOP *r, int chave) {
    if (r == NULL) {
        return NULL;
    }
    if (chave < r->chave) {
        r->esq = remover_p(r->esq, chave);
    } else if (chave > r->chave) {
        r->dir = remover_p(r->dir, chave);
    } else {
        if (r->esq == NULL || r->dir == NULL) {
            NOP *filho = (r->esq != NULL) ? r->esq : r->dir;
            free(r);
            return filho;
        }
        NOP *suc = r->dir;
        while (suc->esq != NULL) {
            suc = suc->esq;
        }
        int chave_suc = suc->chave;
        r->chave = chave_suc;
        r->cont = suc->cont;
        r->dir = remover_p(r->dir, chave_suc);
    }
    return balanceia_p(r);
}

static int buscar_p(NOP *r, int chave) {
    while (r != NULL) {
        if (chave < r->chave) {
            r = r->esq;
        } else if (chave > r->chave) {
            r = r->dir;
        } else {
            return r->cont;
        }
    }
    return 0;
}

static void liberar_p(NOP *r) {
    if (r == NULL) {
        return;
    }
    liberar_p(r->esq);
    liberar_p(r->dir);
    free(r);
}

/* ------------------------------------------------------------------ */
/* AVL do ranking (uma so).                                            */
/* Ordenada por (total decrescente, numero da musica crescente).       */
/* Cada no guarda o tamanho da subarvore para calcular posicoes.       */
/* O total guardado e "armazenado": o total real e total + offset.     */
/* ------------------------------------------------------------------ */

typedef struct no_r NOR;
struct no_r {
    int m;
    long long total;
    int altura;
    int tam;
    NOR *esq;
    NOR *dir;
};

static int altura_r(NOR *n) {
    return n == NULL ? 0 : n->altura;
}

static int tam_r(NOR *n) {
    return n == NULL ? 0 : n->tam;
}

/* a vem antes de b no ranking? */
static int antes(long long ta, int ma, long long tb, int mb) {
    return ta > tb || (ta == tb && ma < mb);
}

static void atualiza_r(NOR *n) {
    int ae = altura_r(n->esq);
    int ad = altura_r(n->dir);
    n->altura = 1 + (ae > ad ? ae : ad);
    n->tam = 1 + tam_r(n->esq) + tam_r(n->dir);
}

static NOR* rot_dir_r(NOR *n) {
    NOR *e = n->esq;
    n->esq = e->dir;
    e->dir = n;
    atualiza_r(n);
    atualiza_r(e);
    return e;
}

static NOR* rot_esq_r(NOR *n) {
    NOR *d = n->dir;
    n->dir = d->esq;
    d->esq = n;
    atualiza_r(n);
    atualiza_r(d);
    return d;
}

static NOR* balanceia_r(NOR *n) {
    atualiza_r(n);
    int fb = altura_r(n->esq) - altura_r(n->dir);

    if (fb > 1) {
        if (altura_r(n->esq->esq) < altura_r(n->esq->dir)) {
            n->esq = rot_esq_r(n->esq);
        }
        return rot_dir_r(n);
    }
    if (fb < -1) {
        if (altura_r(n->dir->dir) < altura_r(n->dir->esq)) {
            n->dir = rot_dir_r(n->dir);
        }
        return rot_esq_r(n);
    }
    return n;
}

static NOR* inserir_r(NOR *r, int m, long long total) {
    if (r == NULL) {
        NOR *n = (NOR *) malloc(sizeof(NOR));
        if (n == NULL) {
            return NULL;
        }
        n->m = m;
        n->total = total;
        n->esq = NULL;
        n->dir = NULL;
        atualiza_r(n);
        return n;
    }
    if (antes(total, m, r->total, r->m)) {
        r->esq = inserir_r(r->esq, m, total);
    } else {
        r->dir = inserir_r(r->dir, m, total);
    }
    return balanceia_r(r);
}

static NOR* remover_r(NOR *r, int m, long long total) {
    if (r == NULL) {
        return NULL;
    }
    if (r->m == m) {
        if (r->esq == NULL || r->dir == NULL) {
            NOR *filho = (r->esq != NULL) ? r->esq : r->dir;
            free(r);
            return filho;
        }
        NOR *suc = r->dir;
        while (suc->esq != NULL) {
            suc = suc->esq;
        }
        int m_suc = suc->m;
        long long total_suc = suc->total;
        r->m = m_suc;
        r->total = total_suc;
        r->dir = remover_r(r->dir, m_suc, total_suc);
    } else if (antes(total, m, r->total, r->m)) {
        r->esq = remover_r(r->esq, m, total);
    } else {
        r->dir = remover_r(r->dir, m, total);
    }
    return balanceia_r(r);
}

/* posicao (a partir de 1) da chave (total, m); 0 se nao achar */
static int posicao_r(NOR *r, int m, long long total) {
    int pos = 0;
    while (r != NULL) {
        if (r->m == m) {
            return pos + tam_r(r->esq) + 1;
        }
        if (antes(total, m, r->total, r->m)) {
            r = r->esq;
        } else {
            pos += tam_r(r->esq) + 1;
            r = r->dir;
        }
    }
    return 0;
}

/* quantos nos tem total armazenado >= v (formam um prefixo do ranking) */
static int conta_maior_igual_r(NOR *r, long long v) {
    int cont = 0;
    while (r != NULL) {
        if (r->total >= v) {
            cont += 1 + tam_r(r->esq);
            r = r->dir;
        } else {
            r = r->esq;
        }
    }
    return cont;
}

static void liberar_r(NOR *r) {
    if (r == NULL) {
        return;
    }
    liberar_r(r->esq);
    liberar_r(r->dir);
    free(r);
}

/* ------------------------------------------------------------------ */
/* TAD                                                                 */
/* ------------------------------------------------------------------ */

struct retrospectiva_ {
    NOP *linha[MAX_USUARIOS + 1];     /* linha[u]  : musicas ouvidas por u (m -> cont) */
    NOP *coluna[MAX_MUSICAS + 1];     /* coluna[m] : usuarios que ouviram m            */
    long long total[MAX_MUSICAS + 1]; /* total armazenado (real = total + offset)      */
    char noranking[MAX_MUSICAS + 1];  /* m esta no ranking?                            */
    NOR *ranking;
    long long offset;                 /* soma dos IMPULSOs                             */
};

RETROSPECTIVA* criar_retrospectiva(void) {
    return (RETROSPECTIVA *) calloc(1, sizeof(RETROSPECTIVA));
}

boolean ouve_retrospectiva(RETROSPECTIVA *r, int u, int m) {
    if (r == NULL || u < 1 || u > MAX_USUARIOS || m < 1 || m > MAX_MUSICAS) {
        return FALSE;
    }

    int novo, ignora;
    r->linha[u] = inserir_p(r->linha[u], m, &novo);
    if (novo) {
        r->coluna[m] = inserir_p(r->coluna[m], u, &ignora);
    }

    if (r->noranking[m]) {
        r->ranking = remover_r(r->ranking, m, r->total[m]);
        r->total[m]++;
    } else {
        r->total[m] = 1 - r->offset;   /* total real = 1, sem impulsos antigos */
        r->noranking[m] = 1;
    }
    r->ranking = inserir_r(r->ranking, m, r->total[m]);
    return TRUE;
}

int quantas_retrospectiva(RETROSPECTIVA *r, int u, int m) {
    if (r == NULL || u < 1 || u > MAX_USUARIOS || m < 1 || m > MAX_MUSICAS) {
        return ERRO;
    }
    return buscar_p(r->linha[u], m);
}

int posicao_retrospectiva(RETROSPECTIVA *r, int m) {
    if (r == NULL || m < 1 || m > MAX_MUSICAS || !r->noranking[m]) {
        return ERRO;
    }
    return posicao_r(r->ranking, m, r->total[m]);
}

int kesima_retrospectiva(RETROSPECTIVA *r, int k) {
    if (r == NULL || k < 1 || k > tam_r(r->ranking)) {
        return ERRO;
    }
    NOR *n = r->ranking;
    while (n != NULL) {
        int e = tam_r(n->esq);
        if (k <= e) {
            n = n->esq;
        } else if (k == e + 1) {
            return n->m;
        } else {
            k -= e + 1;
            n = n->dir;
        }
    }
    return ERRO;
}

int favorita_retrospectiva(RETROSPECTIVA *r, int u) {
    if (r == NULL || u < 1 || u > MAX_USUARIOS || r->linha[u] == NULL) {
        return ERRO;
    }
    return r->linha[u]->melhor_chave;
}

/* Percorre a linha de u: tira as execucoes dos totais e u das colunas. */
static void esquece_percorre(RETROSPECTIVA *r, NOP *n, int u) {
    if (n == NULL) {
        return;
    }
    esquece_percorre(r, n->esq, u);
    esquece_percorre(r, n->dir, u);

    int m = n->chave;
    r->ranking = remover_r(r->ranking, m, r->total[m]);
    r->total[m] -= n->cont;
    if (r->total[m] + r->offset == 0) {   /* so tinha execucoes: sai do ranking */
        r->noranking[m] = 0;
        r->total[m] = 0;
    } else {                              /* ficam os impulsos */
        r->ranking = inserir_r(r->ranking, m, r->total[m]);
    }
    r->coluna[m] = remover_p(r->coluna[m], u);
}

boolean esquece_retrospectiva(RETROSPECTIVA *r, int u) {
    if (r == NULL || u < 1 || u > MAX_USUARIOS) {
        return FALSE;
    }
    esquece_percorre(r, r->linha[u], u);
    liberar_p(r->linha[u]);
    r->linha[u] = NULL;
    return TRUE;
}

boolean impulso_retrospectiva(RETROSPECTIVA *r, int x) {
    if (r == NULL) {
        return FALSE;
    }
    if (r->ranking != NULL) {
        r->offset += x;
    }
    return TRUE;
}

/* Percorre a coluna de m: tira m da linha de cada usuario que a ouviu. */
static void retira_percorre(RETROSPECTIVA *r, NOP *n, int m) {
    if (n == NULL) {
        return;
    }
    retira_percorre(r, n->esq, m);
    retira_percorre(r, n->dir, m);
    r->linha[n->chave] = remover_p(r->linha[n->chave], m);
}

boolean retira_retrospectiva(RETROSPECTIVA *r, int m) {
    if (r == NULL || m < 1 || m > MAX_MUSICAS) {
        return FALSE;
    }
    retira_percorre(r, r->coluna[m], m);
    liberar_p(r->coluna[m]);
    r->coluna[m] = NULL;

    if (r->noranking[m]) {
        r->ranking = remover_r(r->ranking, m, r->total[m]);
        r->noranking[m] = 0;
        r->total[m] = 0;
    }
    return TRUE;
}

int faixa_retrospectiva(RETROSPECTIVA *r, int a, int b) {
    if (r == NULL) {
        return ERRO;
    }
    /* total real >= v  <=>  total armazenado >= v - offset */
    return conta_maior_igual_r(r->ranking, (long long) a - r->offset)
         - conta_maior_igual_r(r->ranking, (long long) b + 1 - r->offset);
}

boolean apagar_retrospectiva(RETROSPECTIVA **r) {
    if (r == NULL || *r == NULL) {
        return FALSE;
    }
    for (int u = 1; u <= MAX_USUARIOS; u++) {
        liberar_p((*r)->linha[u]);
    }
    for (int m = 1; m <= MAX_MUSICAS; m++) {
        liberar_p((*r)->coluna[m]);
    }
    liberar_r((*r)->ranking);
    free(*r);
    *r = NULL;
    return TRUE;
}
