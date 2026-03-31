#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ULA (32 bits correta)
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
        *carry = (A_in + B_in + INC) < A_in;
    }

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

    // Registradores
    int mar=0, mdr=0, pc=0, mbr=0;
    int sp=0, lv=0, cpp=0, tos=0, opc=0, h=0;

    // Leitura dos registradores
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

        // Separar instrução
        char controle[9], c_bus[10], b_bus[5];

        strncpy(controle, linha, 8);
        controle[8] = '\0';

        strncpy(c_bus, linha + 8, 9);
        c_bus[9] = '\0';

        strncpy(b_bus, linha + 17, 4);
        b_bus[4] = '\0';

        // Sinais
        int SLL8 = controle[0] - '0';
        int SRA1 = controle[1] - '0';
        int F0   = controle[2] - '0';
        int F1   = controle[3] - '0';
        int ENA  = controle[4] - '0';
        int ENB  = controle[5] - '0';
        int INVA = controle[6] - '0';
        int INC  = controle[7] - '0';

        // Validação
        if (SLL8 == 1 && SRA1 == 1) {
            printf("> Error, invalid control signals.\n");
            continue;
        }

        // B BUS
        int B = 0;
        int seletor = strtol(b_bus, NULL, 2);

        switch(seletor) {
            case 0: B = mdr; break;
            case 1: B = pc; break;
            case 2: B = mbr; break;
            case 3: B = mbr; break;
            case 4: B = sp; break;
            case 5: B = lv; break;
            case 6: B = cpp; break;
            case 7: B = tos; break;
            case 8: B = opc; break;
        }

        int A = h;

        // PRINT BEFORE
        printf("\n==============================\n");
        printf("Ciclo %d\n", ciclo);
        printf("IR: %s\n", IR);

        printf("\n> Registers BEFORE\n");
        printf("MAR=%d MDR=%d PC=%d MBR=%d\n", mar, mdr, pc, mbr);
        printf("SP=%d LV=%d CPP=%d TOS=%d OPC=%d H=%d\n",
               sp, lv, cpp, tos, opc, h);

        // ULA
        int carry;
        int S = ula(A, B, F0, F1, ENA, ENB, INVA, INC, &carry);

        // SHIFT
        int Sd = S;
        if (SLL8 == 1) Sd = S << 8;
        else if (SRA1 == 1) Sd = S >> 1;

        int n = (Sd < 0) ? 1 : 0;
        int z = (Sd == 0) ? 1 : 0;

        // C BUS
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

        // PRINT AFTER
        printf("\n> Registers AFTER\n");
        printf("MAR=%d MDR=%d PC=%d MBR=%d\n", mar, mdr, pc, mbr);
        printf("SP=%d LV=%d CPP=%d TOS=%d OPC=%d H=%d\n",
               sp, lv, cpp, tos, opc, h);

        printf("A(H)=%d | B=%d | S=%d | Sd=%d | N=%d | Z=%d | Carry=%d\n",
               A, B, S, Sd, n, z, carry);

        ciclo++;
    }

    printf("\nNo more lines, EOP.\n");

    fclose(prog);
    fclose(log);

    return 0;
}