#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LETRAS 15
#define ARQUIVO_LOG "resultado_criptografia.txt"

typedef enum {
    SEQ_PA = 1,
    SEQ_PG,
    SEQ_FIBONACCI,
    SEQ_PRIMOS
} TipoSequencia;

static int ler_linha(const char *mensagem, char *destino, size_t tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        return 0;
    }

    size_t comprimento = strlen(destino);
    if (comprimento > 0 && destino[comprimento - 1] == '\n') {
        destino[comprimento - 1] = '\0';
    } else {
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
            /* descarta o restante da linha */
        }
    }
    return 1;
}

static int ler_inteiro(const char *mensagem, int minimo, int maximo, int *valor) {
    char linha[80];
    char *fim;
    long numero;

    if (!ler_linha(mensagem, linha, sizeof(linha))) {
        return 0;
    }

    errno = 0;
    numero = strtol(linha, &fim, 10);
    while (*fim == ' ' || *fim == '\t') {
        fim++;
    }
    if (linha[0] == '\0' || *fim != '\0' || errno == ERANGE ||
        numero < minimo || numero > maximo) {
        return 0;
    }

    *valor = (int)numero;
    return 1;
}

static int palavra_valida(const char *palavra) {
    size_t tamanho = strlen(palavra);
    if (tamanho == 0 || tamanho > MAX_LETRAS) {
        return 0;
    }
    for (size_t i = 0; i < tamanho; i++) {
        if (!((palavra[i] >= 'A' && palavra[i] <= 'Z') ||
              (palavra[i] >= 'a' && palavra[i] <= 'z'))) {
            return 0;
        }
    }
    return 1;
}

static int eh_primo(int numero) {
    if (numero < 2) {
        return 0;
    }
    for (int divisor = 2; divisor * divisor <= numero; divisor++) {
        if (numero % divisor == 0) {
            return 0;
        }
    }
    return 1;
}

static int primo_na_posicao(int indice) {
    int encontrados = 0;
    for (int numero = 2; ; numero++) {
        if (eh_primo(numero)) {
            if (encontrados == indice) {
                return numero;
            }
            encontrados++;
        }
    }
}

static const char *nome_sequencia(TipoSequencia tipo) {
    switch (tipo) {
        case SEQ_PA: return "PA";
        case SEQ_PG: return "PG";
        case SEQ_FIBONACCI: return "Fibonacci";
        case SEQ_PRIMOS: return "Primos";
        default: return "Desconhecida";
    }
}

static int termo_sequencia(TipoSequencia tipo, int indice, int parametro) {
    switch (tipo) {
        case SEQ_PA:
            /* PA iniciada em 1: 1, 1+r, 1+2r, ... */
            return (1 + indice * parametro) % 26;
        case SEQ_PG: {
            /* PG iniciada em 1; reduzimos módulo 26 para evitar números grandes. */
            int termo = 1;
            for (int i = 0; i < indice; i++) {
                termo = (termo * parametro) % 26;
            }
            return termo;
        }
        case SEQ_FIBONACCI: {
            int anterior = 1;
            int atual = 1;
            if (indice == 0 || indice == 1) {
                return 1;
            }
            for (int i = 2; i <= indice; i++) {
                int proximo = (anterior + atual) % 26;
                anterior = atual;
                atual = proximo;
            }
            return atual;
        }
        case SEQ_PRIMOS:
            return primo_na_posicao(indice) % 26;
        default:
            return 0;
    }
}

int main(void) {
    char palavra[MAX_LETRAS + 2];
    char criptografada[MAX_LETRAS + 1];
    int escolha, shift, parametro = 0;

    printf("=== Cifra de Cesar com sequencia matematica ===\n");
    if (!ler_linha("Digite uma palavra (ate 15 letras, sem acentos ou espacos): ",
                   palavra, sizeof(palavra))) {
        fprintf(stderr, "Erro ao ler a palavra.\n");
        return 1;
    }
    if (!palavra_valida(palavra)) {
        fprintf(stderr, "Entrada invalida: use de 1 a 15 letras A-Z, sem espacos, acentos ou simbolos.\n");
        return 1;
    }

    if (!ler_inteiro("Digite o SHIFT (0 a 1000000): ", 0, 1000000, &shift)) {
        fprintf(stderr, "SHIFT invalido. Informe um numero inteiro entre 0 e 1000000.\n");
        return 1;
    }
    shift %= 26;

    printf("\nEscolha a sequencia:\n");
    printf("1 - Progressao Aritmetica (PA)\n");
    printf("2 - Progressao Geometrica (PG)\n");
    printf("3 - Fibonacci\n");
    printf("4 - Numeros primos\n");
    if (!ler_inteiro("Opcao: ", 1, 4, &escolha)) {
        fprintf(stderr, "Opcao invalida. Escolha de 1 a 4.\n");
        return 1;
    }

    TipoSequencia tipo = (TipoSequencia)escolha;
    if (tipo == SEQ_PA) {
        if (!ler_inteiro("Razao da PA (0 a 1000; exemplo: 2): ", 0, 1000, &parametro)) {
            fprintf(stderr, "Razao invalida.\n");
            return 1;
        }
    } else if (tipo == SEQ_PG) {
        if (!ler_inteiro("Razao da PG (0 a 1000; exemplo: 2): ", 0, 1000, &parametro)) {
            fprintf(stderr, "Razao invalida.\n");
            return 1;
        }
        parametro %= 26;
    }

    size_t tamanho = strlen(palavra);
    int termos[MAX_LETRAS];
    for (size_t i = 0; i < tamanho; i++) {
        int deslocamento_sequencia = termo_sequencia(tipo, (int)i, parametro);
        int deslocamento_total = (shift + deslocamento_sequencia) % 26;
        char letra = palavra[i];
        char base = (letra >= 'A' && letra <= 'Z') ? 'A' : 'a';
        criptografada[i] = (char)(base + (letra - base + deslocamento_total) % 26);
        termos[i] = deslocamento_sequencia;
    }
    criptografada[tamanho] = '\0';

    printf("\nTermos da sequencia (modulo 26): ");
    for (size_t i = 0; i < tamanho; i++) {
        printf("%d%s", termos[i], i + 1 < tamanho ? ", " : "\n");
    }
    printf("Palavra original: %s\n", palavra);
    printf("Palavra criptografada: %s\n", criptografada);

    FILE *arquivo = fopen(ARQUIVO_LOG, "a");
    if (arquivo == NULL) {
        perror("Nao foi possivel gravar o arquivo de resultado");
        return 1;
    }
    fprintf(arquivo,
            "Original: %s | Codificada: %s | SHIFT: %d | Sequencia: %s",
            palavra, criptografada, shift, nome_sequencia(tipo));
    if (tipo == SEQ_PA || tipo == SEQ_PG) {
        fprintf(arquivo, " | Razao: %d", parametro);
    }
    fprintf(arquivo, " | Letras: %zu\n", tamanho);
    fclose(arquivo);

    printf("Resultado registrado em %s\n", ARQUIVO_LOG);
    return 0;
}
