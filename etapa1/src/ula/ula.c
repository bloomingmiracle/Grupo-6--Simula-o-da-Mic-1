#include <stdio.h>
#include <string.h>

// 🟢 PASSO 5 — Função da ULA
int ula(int A, int B, int F0, int F1, int ENA, int ENB, int INVA, int INC, int *carry) {

    int A_in = A;
    int B_in = B;
    int S = 0;

    // ENA / ENB
    if (!ENA) A_in = 0;
    if (!ENB) B_in = 0;

    // INVA
    if (INVA) A_in = ~A_in;

    // Operações
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

    // INC
    if (INC) {
        S = S + 1;
    }

    // Carry
    *carry = 0;
    if (F0 == 1 && F1 == 1) {
        if ((A_in > 0 && B_in > 0 && S < 0) ||
            (A_in < 0 && B_in < 0 && S > 0)) {
            *carry = 1;
        }
    }

    return S;
}

int main() {

    FILE *file = fopen("programa_etapa1.txt", "r");

    FILE *log_file = fopen("saida_etapa1.txt", "w");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de entrada\n");
        return 1;
    }
    if (log_file == NULL) {
        printf("Erro ao criar o arquivo de log\n");
        fclose(file);
        return 1;
    }

    char linha[10];

    // 🟢 Valores iniciais (não interfere nos demais)

    int A = -1;
    int B = 1;
    int PC = 0;

    while (fgets(linha, sizeof(linha), file)) {
        
    linha[strcspn(linha, "\n")] = 0;

        if(strlen(linha) == 0) continue; 

        char IR[10];
        strcpy(IR, linha);

        int F0 = linha[0] - '0';
        int F1 = linha[1] - '0';
        int ENA = linha[2] - '0';
        int ENB = linha[3] - '0';
        int INVA = linha[4] - '0';
        int INC = linha[5] - '0';

        // 🟢 PASSO 6 — Chamada da ULA
        int carry;
        int S = ula(A, B, F0, F1, ENA, ENB, INVA, INC, &carry);

        printf("Cycle (PC): %d | IR: %s | A: %d | B: %d | S: %d | Carry: %d\n", PC, IR, A, B, S, carry);
        
        fprintf(log_file, "Cycle: %d\n", PC);
        fprintf(log_file, "PC: %d\n", PC);
        fprintf(log_file, "IR: %s\n", IR);
        fprintf(log_file, "A: %d\n", A);
        fprintf(log_file, "B: %d\n", B);
        fprintf(log_file, "S: %d\n", S);
        fprintf(log_file, "Carry: %d\n", carry);
        fprintf(log_file, "----------------------\n");

        PC++;
    }

    fclose(file);
    fclose(log_file);
    
    printf("\nProcessamento concluído. Verifique o arquivo 'saida_etapa1.txt'.\n");
    return 0;
}