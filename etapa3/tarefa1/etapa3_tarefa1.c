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

    // Ler linha por linha
    while (fgets(linha, sizeof(linha), dados) && i < 16) {

        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        // Converter binário para inteiro
        memoria[i] = bin_to_int(linha);

        i++;
    }

    fclose(dados);

    // 🔍 TESTE (pode remover depois)
    printf("Memoria carregada:\n");
    for (int j = 0; j < 16; j++) {
        printf("memoria[%d] = %d\n", j, memoria[j]);
    }

    return 0;
}