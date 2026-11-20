#include <stdio.h>
#include <string.h>
#include "retrospectiva.h"

static void imprime(int x) {
    if (x == ERRO) {
        printf("-\n");
    } else {
        printf("%d\n", x);
    }
}

int main(void) {
    char linha[100];
    char comando[50];
    int a, b;
    RETROSPECTIVA *r = criar_retrospectiva();

    if (r == NULL) {
        return 1;
    }

    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        int lidos = sscanf(linha, "%49s %d %d", comando, &a, &b);

        if (lidos < 1) {
            continue;
        }

        if (strcmp(comando, "OUVE") == 0 && lidos == 3) {
            ouve_retrospectiva(r, a, b);
        }
        else if (strcmp(comando, "QUANTAS") == 0 && lidos == 3) {
            imprime(quantas_retrospectiva(r, a, b));
        }
        else if (strcmp(comando, "POSICAO") == 0 && lidos == 2) {
            imprime(posicao_retrospectiva(r, a));
        }
        else if (strcmp(comando, "KESIMA") == 0 && lidos == 2) {
            imprime(kesima_retrospectiva(r, a));
        }
        else if (strcmp(comando, "FAVORITA") == 0 && lidos == 2) {
            imprime(favorita_retrospectiva(r, a));
        }
        else if (strcmp(comando, "ESQUECE") == 0 && lidos == 2) {
            esquece_retrospectiva(r, a);
        }
        else if (strcmp(comando, "IMPULSO") == 0 && lidos == 2) {
            impulso_retrospectiva(r, a);
        }
        else if (strcmp(comando, "RETIRA") == 0 && lidos == 2) {
            retira_retrospectiva(r, a);
        }
        else if (strcmp(comando, "FAIXA") == 0 && lidos == 3) {
            imprime(faixa_retrospectiva(r, a, b));
        }
        else if (strcmp(comando, "FIM") == 0) {
            break;
        }
    }

    apagar_retrospectiva(&r);
    return 0;
}
