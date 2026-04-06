#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para converter binário (string) para inteiro
int bin_to_int(char *bin) {
    return (int)strtol(bin, NULL, 2);
}

// Função para imprimir inteiro em binário (32 bits)
void print_bin32(int value) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

// Função para imprimir inteiro em binário (8 bits)
void print_bin8(int value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

int main() {

    // ============================
    // PASSO 1 — Ler dados (memória)
    // ============================

    FILE *dados = fopen("dados_etapa3_tarefa1.txt", "r");

    if (dados == NULL) {
        printf("Erro ao abrir arquivo de dados\n");
        return 1;
    }

    int memoria[16];
    char linha[40];
    int i = 0;

    while (fgets(linha, sizeof(linha), dados) && i < 16) {
        linha[strcspn(linha, "\n")] = 0;
        memoria[i] = bin_to_int(linha);
        i++;
    }

    fclose(dados);

    // ============================
    // PASSO 3 — Ler registradores
    // ============================

    FILE *regs_file = fopen("registradores_etapa3_tarefa1.txt", "r");

    if (regs_file == NULL) {
        printf("Erro ao abrir arquivo de registradores\n");
        return 1;
    }

    int mar=0, mdr=0, pc=0, mbr=0;
    int sp=0, lv=0, cpp=0, tos=0, opc=0, h=0;

    char reg_name[10];
    char reg_val[40];

    while (fscanf(regs_file, "%s = %s", reg_name, reg_val) == 2) {
        if (strcmp(reg_name, "mar") == 0) mar = bin_to_int(reg_val);
        else if (strcmp(reg_name, "mdr") == 0) mdr = bin_to_int(reg_val);
        else if (strcmp(reg_name, "pc") == 0) pc = bin_to_int(reg_val);
        else if (strcmp(reg_name, "mbr") == 0) mbr = bin_to_int(reg_val);
        else if (strcmp(reg_name, "sp") == 0) sp = bin_to_int(reg_val);
        else if (strcmp(reg_name, "lv") == 0) lv = bin_to_int(reg_val);
        else if (strcmp(reg_name, "cpp") == 0) cpp = bin_to_int(reg_val);
        else if (strcmp(reg_name, "tos") == 0) tos = bin_to_int(reg_val);
        else if (strcmp(reg_name, "opc") == 0) opc = bin_to_int(reg_val);
        else if (strcmp(reg_name, "h") == 0) h = bin_to_int(reg_val);
    }

    fclose(regs_file);

    // ============================
    // PASSO 4 — Estado inicial
    // ============================

    printf("============================================================\n");
    printf("Initial memory state\n");
    printf("*******************************\n");

    for (int j = 0; j < 16; j++) {
        print_bin32(memoria[j]);
        printf("\n");
    }

    printf("*******************************\n");

    printf("Initial register state\n");
    printf("*******************************\n");

    printf("mar = "); print_bin32(mar); printf("\n");
    printf("mdr = "); print_bin32(mdr); printf("\n");
    printf("pc = "); print_bin32(pc); printf("\n");
    printf("mbr = "); print_bin8(mbr); printf("\n");
    printf("sp = "); print_bin32(sp); printf("\n");
    printf("lv = "); print_bin32(lv); printf("\n");
    printf("cpp = "); print_bin32(cpp); printf("\n");
    printf("tos = "); print_bin32(tos); printf("\n");
    printf("opc = "); print_bin32(opc); printf("\n");
    printf("h = "); print_bin32(h); printf("\n");

    printf("============================================================\n");
    printf("Start of Program\n");
    printf("============================================================\n");

    // ============================
    // PASSOS 6, 7 e 8 (estrutura)
    // ============================

    FILE *micro = fopen("microinstrucoes_etapa3_tarefa1.txt", "r");

    if (micro == NULL) {
        printf("Erro ao abrir microinstrucoes\n");
        return 1;
    }

    char instrucao[50];
    int ciclo = 1;

    while (fgets(instrucao, sizeof(instrucao), micro)) {

        instrucao[strcspn(instrucao, "\n")] = 0;

        if (strlen(instrucao) == 0) continue;

        printf("Cycle %d\n", ciclo);
        printf("IR: %s\n", instrucao);

        // BEFORE
        printf("\n> Registers before instruction\n");
        printf("mar = "); print_bin32(mar); printf("\n");
        printf("mdr = "); print_bin32(mdr); printf("\n");
        printf("pc = "); print_bin32(pc); printf("\n");
        printf("mbr = "); print_bin8(mbr); printf("\n");
        printf("sp = "); print_bin32(sp); printf("\n");
        printf("lv = "); print_bin32(lv); printf("\n");
        printf("cpp = "); print_bin32(cpp); printf("\n");
        printf("tos = "); print_bin32(tos); printf("\n");
        printf("opc = "); print_bin32(opc); printf("\n");
        printf("h = "); print_bin32(h); printf("\n");

        // (Ainda sem execução real — só estrutura)

        // AFTER
        printf("\n> Registers after instruction\n");
        printf("mar = "); print_bin32(mar); printf("\n");
        printf("mdr = "); print_bin32(mdr); printf("\n");
        printf("pc = "); print_bin32(pc); printf("\n");
        printf("mbr = "); print_bin8(mbr); printf("\n");
        printf("sp = "); print_bin32(sp); printf("\n");
        printf("lv = "); print_bin32(lv); printf("\n");
        printf("cpp = "); print_bin32(cpp); printf("\n");
        printf("tos = "); print_bin32(tos); printf("\n");
        printf("opc = "); print_bin32(opc); printf("\n");
        printf("h = "); print_bin32(h); printf("\n");

        // PASSO 7 — Memória em binário (CORRIGIDO)
        printf("\n> Memory after instruction\n");
        printf("*******************************\n");
        for (int k = 0; k < 16; k++) {
            print_bin32(memoria[k]);
            printf("\n");
        }

        printf("============================================================\n");

        ciclo++;
    }

    // PASSO 8
    printf("No more lines, EOP.\n");

    fclose(micro);

    return 0;
}
