#ifndef RETROSPECTIVA_H
#define RETROSPECTIVA_H

#define FALSE 0
#define TRUE 1
#define ERRO -32000
#define boolean int

#define MAX_USUARIOS 100000
#define MAX_MUSICAS 100000

typedef struct retrospectiva_ RETROSPECTIVA;

RETROSPECTIVA* criar_retrospectiva(void);
boolean ouve_retrospectiva(RETROSPECTIVA *r, int u, int m);
int quantas_retrospectiva(RETROSPECTIVA *r, int u, int m);
int posicao_retrospectiva(RETROSPECTIVA *r, int m);
int kesima_retrospectiva(RETROSPECTIVA *r, int k);
int favorita_retrospectiva(RETROSPECTIVA *r, int u);
boolean esquece_retrospectiva(RETROSPECTIVA *r, int u);
boolean impulso_retrospectiva(RETROSPECTIVA *r, int x);
boolean retira_retrospectiva(RETROSPECTIVA *r, int m);
int faixa_retrospectiva(RETROSPECTIVA *r, int a, int b);
boolean apagar_retrospectiva(RETROSPECTIVA **r);

#endif
