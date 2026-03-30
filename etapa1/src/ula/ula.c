#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// PASSO 5 — Função da ULA
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

    // Carry (forma simplificada para esta etapa)
    *carry = 0;
    if (F0 == 1 && F1 == 1) {
        *carry = (A_in + B_in + INC) > 1;
    }

    return S;
}

int main() {

    // Registradores
    int mar = 0, mdr = 0, pc = 0, mbr = 0;
    int sp = 0, lv = 0, cpp = 0, tos = 0, opc = 0, h = 0;

    FILE *file = fopen("programa_etapa2_tarefa2.txt", "r");
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

    char linha[30];

    // Valores iniciais (conforme enunciado)
    int A = -1;
    int B = 1;
    int PC = 0;

    // Leitura de dados, pode conter erros ainda
    FILE *reg_file = fopen("registradores_etapa2_tarefa2.txt", "r");

    if (reg_file == NULL) {
        printf("Erro ao abrir arquivo de registradores\n");
        return 1;
    }

    char nome[10];
    int valor;

    while (fscanf(reg_file, "%s %d", nome, &valor) != EOF) {

        if (strcmp(nome, "MAR") == 0) mar = valor;
        else if (strcmp(nome, "MDR") == 0) mdr = valor;
        else if (strcmp(nome, "PC") == 0) pc = valor;
        else if (strcmp(nome, "MBR") == 0) mbr = valor;
        else if (strcmp(nome, "SP") == 0) sp = valor;
        else if (strcmp(nome, "LV") == 0) lv = valor;
        else if (strcmp(nome, "CPP") == 0) cpp = valor;
        else if (strcmp(nome, "TOS") == 0) tos = valor;
        else if (strcmp(nome, "OPC") == 0) opc = valor;
        else if (strcmp(nome, "H") == 0) h = valor;
    }

    fclose(reg_file);

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

        printf("\nInstrucao: %s", linha);
        printf("\nF0=%d F1=%d ENA=%d ENB=%d INVA=%d INC=%d\n", F0, F1, ENA, ENB, INVA, INC);

        // 🟢 PASSO 6 — Chamada da ULA
        int carry;
        int S = ula(A, B, F0, F1, ENA, ENB, INVA, INC, &carry);

        // 🟢 PASSO 7 — Atualização de A
        int LogA = A; // valor antigo de A
        A = S;

        printf("Cycle (PC): %d | IR: %s | A: %d | B: %d | S: %d | Carry: %d\n", PC, IR, LogA, B, S, carry);

        fprintf(log_file, "Cycle: %d\n", PC);
        fprintf(log_file, "PC: %d\n", PC);
        fprintf(log_file, "IR: %s\n", IR);
        fprintf(log_file, "A: %d\n", LogA);
        fprintf(log_file, "B: %d\n", B);
        fprintf(log_file, "S: %d\n", S);
        fprintf(log_file, "Carry: %d\n", carry);
        fprintf(log_file, "----------------------\n");

        PC++;
    }

    fclose(file);
    fclose(log_file);

    printf("\nProcessamento concluido. Verifique o arquivo 'saida_etapa1.txt'.\n");
    return 0;
}
