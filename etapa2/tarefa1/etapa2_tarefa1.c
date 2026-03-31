#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Função auxiliar para imprimir valores em binário de 32 bits
void print_bin(const char* name, int value) {
    printf("%s = ", name);
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

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

    char linha[20];

    int a = 1;
    int b = 0x80000000; // Representa o limite mínimo negativo (1000...0000 em binário)
    int PC = 1;

    print_bin("b", b);
    print_bin("a", a);
    printf("\nStart of Program\n");

    while (fgets(linha, sizeof(linha), file)) {

        linha[strcspn(linha, "\n")] = 0;
        linha[strcspn(linha, "\r")] = 0;

        printf("============================================================\n");
        printf("Ciclo %d\n\n", PC);
        printf("PC = %d\n", PC);

        if (strlen(linha) < 8) {
            printf("> Linha vazia, EOP.\n");
            break;
        }

        printf("IR = %s\n", linha);

        int SLL8 = linha[0] - '0';
        int SRA1 = linha[1] - '0';
        int F0   = linha[2] - '0';
        int F1   = linha[3] - '0';
        int ENA  = linha[4] - '0';
        int ENB  = linha[5] - '0';
        int INVA = linha[6] - '0';
        int INC  = linha[7] - '0';

        if (SLL8 == 1 && SRA1 == 1) {
            printf("> Erro. Sinais de controle invalidos.\n");
            PC++;
            continue;
        }

        int carry;
        int s = ula(a, b, F0, F1, ENA, ENB, INVA, INC, &carry);

        int n = (s < 0) ? 1 : 0;
        int z = (s == 0) ? 1 : 0;

        int sd = s;
        if (SLL8 == 1) {
            sd = s << 8;
        } else if (SRA1 == 1) {
            sd = s >> 1;
        }

        print_bin("b", b);
        print_bin("a", a);
        print_bin("s", s);
        print_bin("sd", sd);
        printf("n = %d\n", n);
        printf("z = %d\n", z);
        printf("co = %d\n", carry);

        a = s;

        PC++;
    }

    fclose(file);
    return 0;
}
