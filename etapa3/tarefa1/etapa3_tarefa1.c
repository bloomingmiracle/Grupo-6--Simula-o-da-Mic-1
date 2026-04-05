#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para converter binário (string) para inteiro
int bin_to_int(char *bin) {
    return (int)strtol(bin, NULL, 2);
}

int main() {

    // PASSO 1 — Abrir arquivo de dados (memória)
    FILE *dados = fopen("dados_etapa3_tarefa1.txt", "r");

    if (dados == NULL) {
        printf("Erro ao abrir arquivo de dados\n");
        return 1;
    }

    // PASSO 2 — Criar memória
    int memoria[16];

    char linha[40];
    int i = 0;

    while (fgets(linha, sizeof(linha), dados) && i < 16) {
        linha[strcspn(linha, "\n")] = 0;
        memoria[i] = bin_to_int(linha);
        i++;
    }

    fclose(dados);
    
    FILE *regs_file = fopen("registradores_etapa3_tarefa1.txt", "r");
    if (regs_file == NULL) {
        printf("Erro ao abrir arquivo de registradores\n");
        return 1;
    }

    int mar = 0, mdr = 0, pc = 0, mbr = 0, sp = 0;
    int lv = 0, cpp = 0, tos = 0, opc = 0, h = 0;

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

    printf("\n*******************************\n");
    printf("Estado inicial da memoria\n");
    printf("*******************************\n");

    for (int j = 0; j < 16; j++) {
        for (int b = 31; b >= 0; b--) {
            printf("%d", (memoria[j] >> b) & 1);
        }
        printf("\n");
    }

    printf("\n*******************************\n");
    printf("Estado inicial do registrador\n");
    printf("*******************************\n");

    printf("mar = ");
    for (int b = 31; b >= 0; b--) printf("%d", (mar >> b) & 1);
    printf("\n");

    printf("mdr = ");
    for (int b = 31; b >= 0; b--) printf("%d", (mdr >> b) & 1);
    printf("\n");

    printf("pc = ");
    for (int b = 31; b >= 0; b--) printf("%d", (pc >> b) & 1);
    printf("\n");

    printf("mbr = ");
    for (int b = 7; b >= 0; b--) printf("%d", (mbr >> b) & 1);
    printf("\n");

    printf("sp = ");
    for (int b = 31; b >= 0; b--) printf("%d", (sp >> b) & 1);
    printf("\n");

    printf("lv = ");
    for (int b = 31; b >= 0; b--) printf("%d", (lv >> b) & 1);
    printf("\n");

    printf("cpp = ");
    for (int b = 31; b >= 0; b--) printf("%d", (cpp >> b) & 1);
    printf("\n");

    printf("tos = ");
    for (int b = 31; b >= 0; b--) printf("%d", (tos >> b) & 1);
    printf("\n");

    printf("opc = ");
    for (int b = 31; b >= 0; b--) printf("%d", (opc >> b) & 1);
    printf("\n");

    printf("h = ");
    for (int b = 31; b >= 0; b--) printf("%d", (h >> b) & 1);
    printf("\n");

    // ================================
    // 🔥 PASSOS 6, 7 e 8 (ADICIONADOS)
    // ================================

    FILE *micro = fopen("microinstrucoes_etapa3_tarefa1.txt", "r");

    if (micro == NULL) {
        printf("Erro ao abrir microinstrucoes\n");
        return 1;
    }

    char instrucao[50];
    int ciclo = 0;

    while (fgets(instrucao, sizeof(instrucao), micro)) {

        instrucao[strcspn(instrucao, "\n")] = 0;

        if (strlen(instrucao) == 0) continue;

        printf("\n============================\n");
        printf("Ciclo %d\n", ciclo);
        printf("IR: %s\n", instrucao);

        // BEFORE
        printf("\n> Registradores ANTES\n");
        printf("MAR=%d MDR=%d PC=%d MBR=%d\n", mar, mdr, pc, mbr);
        printf("SP=%d LV=%d CPP=%d TOS=%d OPC=%d H=%d\n",
               sp, lv, cpp, tos, opc, h);

        

        // 🟢 PASSO 6 — SIMULAÇÃO DE MEMÓRIA (TEMPORÁRIO)
        // leitura exemplo
        mdr = memoria[mar];

        // escrita exemplo (comentado por enquanto)
        // memoria[mar] = mdr;

        // AFTER
        printf("\n> Registradores DEPOIS\n");
        printf("MAR=%d MDR=%d PC=%d MBR=%d\n", mar, mdr, pc, mbr);
        printf("SP=%d LV=%d CPP=%d TOS=%d OPC=%d H=%d\n",
               sp, lv, cpp, tos, opc, h);

        //  PASSO 7 — MEMÓRIA
        printf("\nMemoria:\n");
        for (int k = 0; k < 16; k++) {
            printf("[%d]=%d ", k, memoria[k]);
        }
        printf("\n");

        ciclo++;
    }

    // 🟢 PASSO 8 
    printf("\nNo more lines, EOP.\n");

    fclose(micro);

    return 0;
}