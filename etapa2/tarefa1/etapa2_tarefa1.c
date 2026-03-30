#include <stdio.h>

// PASSO 5 — Funcao da ULA
int ula(int A, int B, int F0, int F1, int ENA, int ENB, int INVA, int INC, int *carry) {

    int A_in = A;
    int B_in = B;
    int S = 0;

    if (!ENA) A_in = 0;
    if (!ENB) B_in = 0;

    if (INVA) A_in = ~A_in;

    if (F0 == 0 && F1 == 0) {
        S = A_in & B_in;
    }
    else if (F0 == 0 && F1 == 1) {
        S = A_in | B_in;
    }
    else if (F0 == 1 && F1 == 0) {
        S = ~B_in;
    }
    else if (F0 == 1 && F1 == 1) {
        S = A_in + B_in;
    }

    if (INC) {
        S = S + 1;
    }

    *carry = 0;
    if (F0 == 1 && F1 == 1) {
        *carry = (A_in + B_in + INC) > 1;
    }

    return S;
}

int main() {

    FILE *file = fopen("programa_etapa2_tarefa1.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    char linha[10];

    int A = -1;
    int B = 1;
    int PC = 1;

    while (fgets(linha, sizeof(linha), file)) {

        int F0 = linha[0] - '0';
        int F1 = linha[1] - '0';
        int ENA = linha[2] - '0';
        int ENB = linha[3] - '0';
        int INVA = linha[4] - '0';
        int INC = linha[5] - '0';

        int carry;
        int S = ula(A, B, F0, F1, ENA, ENB, INVA, INC, &carry);

        int n = (S < 0);
        int z = (S == 0);
        int sd = S << 1;

        printf("\nCycle %d\n", PC);
        printf("Instrucao: %s", linha);
        printf("A: %d | B: %d\n", A, B);
        printf("S: %d | sd: %d | n: %d | z: %d | co: %d\n", S, sd, n, z, carry);

        A = S;

        PC++;
    }

    fclose(file);

    printf("\nFim do programa.\n");
    return 0;
}
