#include <stdio.h>

int main() {

    FILE *file = fopen("programa_etapa1.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    char linha[10];

    while (fgets(linha, sizeof(linha), file)) {

    int F0 = linha[0] - '0';
    int F1 = linha[1] - '0';
    int ENA = linha[2] - '0';
    int ENB = linha[3] - '0';
    int INVA = linha[4] - '0';
    int INC = linha[5] - '0';

    printf("Instrucao: %s", linha);
    printf("F0=%d F1=%d ENA=%d ENB=%d INVA=%d INC=%d\n\n",
           F0, F1, ENA, ENB, INVA, INC);
}
}
