#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================
// Conversões binárias
// ============================

int bin_to_int(char *bin) {
    return (int)strtol(bin, NULL, 2);
}

void print_bin32(int value) {
    for (int i = 31; i >= 0; i--)
        printf("%d", (value >> i) & 1);
}

void print_bin8(int value) {
    for (int i = 7; i >= 0; i--)
        printf("%d", (value >> i) & 1);
}

// ============================
// ULA
// ============================

int ula(int A, int B, int F0, int F1, int ENA, int ENB, int INVA, int INC, int *carry) {

    int A_in = ENA ? A : 0;
    int B_in = ENB ? B : 0;

    if (INVA) A_in = ~A_in;

    int S = 0;

    if (F0 == 0 && F1 == 0) S = A_in & B_in;
    else if (F0 == 0 && F1 == 1) S = A_in | B_in;
    else if (F0 == 1 && F1 == 0) S = ~B_in;
    else if (F0 == 1 && F1 == 1) S = A_in + B_in;

    if (INC) S++;

    *carry = (F0 == 1 && F1 == 1) ? (A_in + B_in + INC > 1) : 0;

    return S;
}

// ============================
// MAIN
// ============================

int main() {

    // Memória
    int memoria[16];

    FILE *dados = fopen("dados_etapa3_tarefa1.txt", "r");
    if (!dados) {
        printf("Erro ao abrir dados\n");
        return 1;
    }

    char linha[50];
    int i = 0;

    while (fgets(linha, sizeof(linha), dados) && i < 16) {
        linha[strcspn(linha, "\n")] = 0;
        memoria[i] = bin_to_int(linha);
        i++;
    }

    fclose(dados);

    // Registradores
    int mar=0, mdr=0, pc=0, mbr=0;
    int sp=0, lv=0, cpp=0, tos=0, opc=0, h=0;

    FILE *regs = fopen("registradores_etapa3_tarefa1.txt", "r");
    char nome[10], valor[40];

    while (fscanf(regs, "%s = %s", nome, valor) == 2) {
        if (!strcmp(nome,"mar")) mar = bin_to_int(valor);
        else if (!strcmp(nome,"mdr")) mdr = bin_to_int(valor);
        else if (!strcmp(nome,"pc")) pc = bin_to_int(valor);
        else if (!strcmp(nome,"mbr")) mbr = bin_to_int(valor);
        else if (!strcmp(nome,"sp")) sp = bin_to_int(valor);
        else if (!strcmp(nome,"lv")) lv = bin_to_int(valor);
        else if (!strcmp(nome,"cpp")) cpp = bin_to_int(valor);
        else if (!strcmp(nome,"tos")) tos = bin_to_int(valor);
        else if (!strcmp(nome,"opc")) opc = bin_to_int(valor);
        else if (!strcmp(nome,"h")) h = bin_to_int(valor);
    }

    fclose(regs);

    // Estado inicial
    printf("==== INITIAL STATE ====\n");

    printf("Memory:\n");
    for(int j=0;j<16;j++){
        print_bin32(memoria[j]);
        printf("\n");
    }

    printf("\nRegisters:\n");
    printf("H="); print_bin32(h); printf("\n");

    // Microinstruções
    FILE *micro = fopen("microinstrucoes_etapa3_tarefa1.txt", "r");
    int ciclo = 1;

    char *b_nome[] = {"MDR","PC","MBR","MBRU","SP","LV","CPP","TOS","OPC"};
    char *c_nome[] = {"MAR","MDR","PC","SP","LV","CPP","TOS","OPC","H"};

    while (fgets(linha, sizeof(linha), micro)) {

        linha[strcspn(linha, "\n")] = 0;
        if (!strlen(linha)) continue;

        printf("\n============================\n");
        printf("Cycle %d\n", ciclo);
        printf("IR: %s\n", linha);

        // BEFORE
        printf("\n> BEFORE\n");
        printf("H="); print_bin32(h); printf("\n");
        printf("MDR="); print_bin32(mdr); printf("\n");
        printf("MAR="); print_bin32(mar); printf("\n");

        // Decode
        int SLL8=linha[0]-'0', SRA1=linha[1]-'0';
        int F0=linha[2]-'0', F1=linha[3]-'0';
        int ENA=linha[4]-'0', ENB=linha[5]-'0';
        int INVA=linha[6]-'0', INC=linha[7]-'0';

        int C[9];
        for(int i=0;i<9;i++) C[i]=linha[8+i]-'0';

        int WRITE=linha[17]-'0', READ=linha[18]-'0';

        int Bsel=(linha[19]-'0')*8+(linha[20]-'0')*4+
                 (linha[21]-'0')*2+(linha[22]-'0');

        // FETCH
        if (WRITE && READ) {
            int valor=0;
            for(int i=0;i<8;i++)
                valor=(valor<<1)|(linha[i]-'0');

            mbr = valor;
            h = valor;

            printf("FETCH executed\n");

            ciclo++;
            continue;
        }

        // B-bus
        int B;
        switch(Bsel){
            case 0: B=mdr; break;
            case 1: B=pc; break;
            case 2: B=(signed char)mbr; break;
            case 3: B=(unsigned char)mbr; break;
            case 4: B=sp; break;
            case 5: B=lv; break;
            case 6: B=cpp; break;
            case 7: B=tos; break;
            case 8: B=opc; break;
            default: B=0;
        }

        // ULA
        int carry;
        int S = ula(h, B, F0,F1,ENA,ENB,INVA,INC,&carry);

        // SHIFT
        int Sd=S;
        if (SLL8) Sd=S<<8;
        else if (SRA1) Sd=S>>1;

        // C-bus
        if (C[8]) h=Sd;
        if (C[7]) opc=Sd;
        if (C[6]) tos=Sd;
        if (C[5]) cpp=Sd;
        if (C[4]) lv=Sd;
        if (C[3]) sp=Sd;
        if (C[2]) pc=Sd;
        if (C[1]) mdr=Sd;
        if (C[0]) mar=Sd;

        // Memória
        if (mar>=0 && mar<16){
            if (WRITE) memoria[mar]=mdr;
            if (READ)  mdr=memoria[mar];
        }

        // AFTER
        printf("\n> AFTER\n");
        printf("H="); print_bin32(h); printf("\n");
        printf("MDR="); print_bin32(mdr); printf("\n");
        printf("MAR="); print_bin32(mar); printf("\n");

        // Info datapath
        printf("\nB-bus: %s\n", (Bsel<=8)?b_nome[Bsel]:"?");
        printf("C-bus: ");
        for(int i=0;i<9;i++)
            if(C[i]) printf("%s ", c_nome[i]);
        printf("\n");

        // Memória
        printf("\nMemory:\n");
        for(int k=0;k<16;k++){
            print_bin32(memoria[k]);
            printf("\n");
        }

        ciclo++;
    }

    printf("\nNo more lines, EOP.\n");

    fclose(micro);
    return 0;
}