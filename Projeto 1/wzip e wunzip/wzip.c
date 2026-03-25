#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    int i;
    int contador = 0;
    char atual, c;

    for (i = 1; i < argc; i++) {

        FILE *arquivo = fopen(argv[i], "r");

        if (arquivo == NULL) {
            printf("wzip: cannot open file\n");
            exit(1);
        }

        while ((c = fgetc(arquivo)) != EOF) {

            if (contador == 0) {
                atual = c;
                contador = 1;
            }
            else if (c == atual) {
                contador++;
            }
            else {
                fwrite(&contador, sizeof(int), 1, stdout);
                fwrite(&atual, sizeof(char), 1, stdout);

                atual = c;
                contador = 1;
            }
        }

        fclose(arquivo);
    }

    if (contador > 0) {
        fwrite(&contador, sizeof(int), 1, stdout);
        fwrite(&atual, sizeof(char), 1, stdout);
    }

    return 0;
}
