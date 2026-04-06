#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Memória
int memoria[8];

// Função para carregar memória
void carregar_memoria() {
    FILE *f = fopen("dados_etapa3_tarefa1.txt", "r");
    if (!f) {
        printf("Erro ao abrir memoria\n");
        exit(1);
    }

    for (int i = 0; i < 8; i++) {
        fscanf(f, "%d", &memoria[i]);
    }

    fclose(f);
}


// Função da ULA
int ula(int A, int B, int F0, int F1, int ENA, int ENB, int INVA, int INC, int *carry) {

    int A_in = ENA ? A : 0;
    int B_in = ENB ? B : 0;
    if (INVA) A_in = ~A_in;
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

    // Carry (forma simplificada)
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

    carregar_memoria();

    FILE *file = fopen("microinstrucoes_etapa3_tarefa1.txt", "r");
    if (!file) {
        printf("Erro ao abrir microinstrucoes\n");
        return 1;
    }

    FILE *log_file = fopen("saida_etapa3.txt", "w");
    if (log_file == NULL) {
        printf("Erro ao criar o arquivo de log\n");
        fclose(file);
        return 1;
    }

    FILE *reg_file = fopen("registradores_etapa3_tarefa1.txt", "r");
    if (!reg_file) {
        printf("Erro ao abrir registradores\n");
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

    char linha[40];
    int ciclo = 0;

    while (fgets(linha, sizeof(linha), file)) {

        linha[strcspn(linha, "\n")] = 0;
        if (strlen(linha) == 0) continue;

        printf("\n============================\n");
        printf("CICLO %d\n", ciclo);
        printf("Instrucao: %s\n", linha);

        // 🔹 ULA (8 bits)
        int SLL8 = linha[0] - '0';
        int SRA1 = linha[1] - '0';
        int F0   = linha[2] - '0';
        int F1   = linha[3] - '0';
        int ENA  = linha[4] - '0';
        int ENB  = linha[5] - '0';
        int INVA = linha[6] - '0';
        int INC  = linha[7] - '0';

        // 🔹 C-bus (9 bits)
        int C[9];
        for(int i = 0; i < 9; i++)
            C[i] = linha[8 + i] - '0';

        // 🔹 Memória
        int WRITE = linha[17] - '0';
        int READ  = linha[18] - '0';

        // 🔹 B-bus
        int Bsel = (linha[19]-'0')*8 +
                   (linha[20]-'0')*4 +
                   (linha[21]-'0')*2 +
                   (linha[22]-'0');

        // 🔹 Seleção do B
        int B;
        switch(Bsel) {
            case 0: B = mdr; break;
            case 1: B = pc; break;
            case 2: B = mbr; break;
            case 3: B = mbr; break;
            case 4: B = sp; break;
            case 5: B = lv; break;
            case 6: B = cpp; break;
            case 7: B = tos; break;
            case 8: B = opc; break;
            default: B = 0;
        }

        // ULA
        int carry;
        int S = ula(h, B, F0, F1, ENA, ENB, INVA, INC, &carry);

        // SHIFT
        int Sd = S;
        if (SLL8) Sd = S << 8;
        if (SRA1) Sd = S >> 1;

        // Atualização C-bus
        if (C[8]) h   = Sd;
        if (C[7]) opc = Sd;
        if (C[6]) tos = Sd;
        if (C[5]) cpp = Sd;
        if (C[4]) lv  = Sd;
        if (C[3]) sp  = Sd;
        if (C[2]) pc  = Sd;
        if (C[1]) mdr = Sd;
        if (C[0]) mar = Sd;

        // MEMÓRIA (DEPOIS DO C-BUS)
        if (WRITE) {
            memoria[mar] = mdr;
        }

        if (READ) {
            mdr = memoria[mar];
        }

        // 🔹 LOG
        printf("H=%d OPC=%d TOS=%d CPP=%d LV=%d SP=%d PC=%d MDR=%d MAR=%d\n",
               h, opc, tos, cpp, lv, sp, pc, mdr, mar);

        printf("B veio de: %d | S=%d | Sd=%d\n", Bsel, S, Sd);

        printf("Memoria:\n");
        for(int i=0;i<8;i++)
            printf("[%d]=%d ", i, memoria[i]);
        printf("\n");

        ciclo++;
    }

    fclose(file);

    printf("\nExecucao concluida.\n");
    return 0;
}