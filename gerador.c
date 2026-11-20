#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint64_t estado;

static uint64_t proximo(void) {
    estado ^= estado >> 12;
    estado ^= estado << 25;
    estado ^= estado >> 27;
    return estado * 2685821657736338717ULL;
}

static long sorteia(long n) { return 1 + (long)(proximo() % (uint64_t)n); }

int main(int argc, char **argv) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Uso: %s <POSICAO|KESIMA|FAVORITA|IMPULSO|RETIRA> <n> [semente]\n", argv[0]);
        return 1;
    }

    const char *rajada = argv[1];
    char *resto;
    long n = strtol(argv[2], &resto, 10);
    if (*resto != '\0' || n < 1 || n > 100000) {
        fprintf(stderr, "n invalido: %s\n", argv[2]);
        return 1;
    }
    if (strcmp(rajada, "POSICAO") && strcmp(rajada, "KESIMA") && strcmp(rajada, "FAVORITA") &&
        strcmp(rajada, "IMPULSO") && strcmp(rajada, "RETIRA")) {
        fprintf(stderr, "Rajada desconhecida: %s\n", rajada);
        return 1;
    }

    estado = (argc == 4) ? strtoull(argv[3], NULL, 10) : 88172645463325252ULL;
    if (estado == 0) estado = 1;

    /* n comandos OUVE u m */
    for (long i = 0; i < n; i++) {
        printf("OUVE %ld %ld\n", sorteia(n), sorteia(n));
    }

    /* n comandos da rajada */
    for (long i = 0; i < n; i++) {
        if (!strcmp(rajada, "POSICAO")) {
            printf("POSICAO %ld\n", sorteia(n));
        } else if (!strcmp(rajada, "KESIMA")) {
            printf("KESIMA %ld\n", sorteia(n));
        } else if (!strcmp(rajada, "FAVORITA")) {
            printf("FAVORITA %ld\n", sorteia(n));
        } else if (!strcmp(rajada, "IMPULSO")) {
            if (i % 2 == 0) printf("IMPULSO %ld\n", sorteia(1000));
            else printf("KESIMA %ld\n", sorteia(n));
        } else { /* RETIRA */
            if (i % 2 == 0) printf("RETIRA %ld\n", sorteia(n));
            else printf("FAVORITA %ld\n", sorteia(n));
        }
    }

    printf("FIM\n");
    return 0;
}
