#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 🟢 ULA
int ula(int A, int B, int F0, int F1, int ENA, int ENB, int INVA, int INC, int *carry) {

    int A_in = A & 1;
    int B_in = B & 1;
    int S = 0;

    if (!ENA) A_in = 0;
    if (!ENB) B_in = 0;

    if (INVA) A_in = (~A_in) & 1;

    if (F0 == 0 && F1 == 0) {
        S = A_in & B_in;
    }
    else if (F0 == 0 && F1 == 1) {
        S = A_in | B_in;
    }
    else if (F0 == 1 && F1 == 0) {
        S = (~B_in) & 1;
    }
    else if (F0 == 1 && F1 == 1) {
        S = A_in + B_in + INC;
    }

    *carry = (F0 == 1 && F1 == 1) ? (S > 1) : 0;

    S &= 1;

    return S;
}

int main() {

    FILE *prog = fopen("programa_etapa2_tarefa2.txt", "r");
    FILE *regs = fopen("registradores_etapa2_tarefa2.txt", "r");
    FILE *log = fopen("saida.txt", "w");

    if (!prog || !regs || !log) {
        printf("Erro ao abrir arquivos\n");
        return 1;
    }

    // 🔵 REGISTRADORES
    int mar=0, mdr=0, pc=0, mbr=0;
    int sp=0, lv=0, cpp=0, tos=0, opc=0, h=0;

    // 🔵 LEITURA DOS REGISTRADORES
    char nome[10];
    int valor;

    while (fscanf(regs, "%s %d", nome, &valor) != EOF) {
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

    fclose(regs);

    char linha[30];
    int ciclo = 0;

    while (fgets(linha, sizeof(linha), prog)) {

        linha[strcspn(linha, "\n")] = 0;
        if (strlen(linha) != 21) continue;

        char IR[30];
        strcpy(IR, linha);

        // 🔵 ETAPA 5 — Separar instrução
        char controle[9], c_bus[10], b_bus[5];

        strncpy(controle, linha, 8);
        controle[8] = '\0';

        strncpy(c_bus, linha + 8, 9);
        c_bus[9] = '\0';

        strncpy(b_bus, linha + 17, 4);
        b_bus[4] = '\0';

        // 🔵 Sinais da ULA
        int SLL8 = controle[0] - '0';
        int SRA1 = controle[1] - '0';
        int F0   = controle[2] - '0';
        int F1   = controle[3] - '0';
        int ENA  = controle[4] - '0';
        int ENB  = controle[5] - '0';
        int INVA = controle[6] - '0';
        int INC  = controle[7] - '0';

        // 🔵 B BUS (decodificador)
        int B = 0;
        int seletor = strtol(b_bus, NULL, 2);

        switch(seletor) {
            case 0: B = mdr; break;
            case 1: B = pc; break;
            case 2: B = mbr; break;
            case 3: B = mbr; break; // simplificado
            case 4: B = sp; break;
            case 5: B = lv; break;
            case 6: B = cpp; break;
            case 7: B = tos; break;
            case 8: B = opc; break;
        }

        // 🔵 Entrada A da ULA
        int A = h;

        int carry;
        int S = ula(A, B, F0, F1, ENA, ENB, INVA, INC, &carry);

        int Sd = S;

        // 🔵 C BUS (escrita nos registradores)
        for (int i = 0; i < 9; i++) {
            if (c_bus[i] == '1') {
                switch(8 - i) {
                    case 8: h = Sd; break;
                    case 7: opc = Sd; break;
                    case 6: tos = Sd; break;
                    case 5: cpp = Sd; break;
                    case 4: lv = Sd; break;
                    case 3: sp = Sd; break;
                    case 2: pc = Sd; break;
                    case 1: mdr = Sd; break;
                    case 0: mar = Sd; break;
                }
            }
        }

        // 🔵 LOG
        printf("\nCiclo %d | IR: %s\n", ciclo, IR);
        printf("B selecionado: %d\n", seletor);
        printf("A(H): %d | B: %d | S: %d | Carry: %d\n", A, B, S, carry);

        fprintf(log, "Ciclo %d\n", ciclo);
        fprintf(log, "IR: %s\n", IR);
        fprintf(log, "H=%d OPC=%d TOS=%d CPP=%d LV=%d SP=%d PC=%d MDR=%d MAR=%d\n",
                h, opc, tos, cpp, lv, sp, pc, mdr, mar);
        fprintf(log, "----------------------\n");

        ciclo++;
    }

    fclose(prog);
    fclose(log);

    printf("\nExecucao concluida.\n");

    return 0;
}