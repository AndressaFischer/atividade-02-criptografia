# Projeto: Cifra de César com sequência matemática

Projeto didático em C que combina a cifra de César com uma sequência matemática. Foi mantido intencionalmente simples para servir como base do trabalho final.

## O que o programa faz

1. Recebe uma palavra de 1 a 15 letras, sem espaços, acentos ou símbolos.
2. Recebe um valor de SHIFT.
3. Permite escolher uma sequência: PA, PG, Fibonacci ou números primos.
4. Para cada letra, soma o SHIFT ao termo correspondente da sequência e desloca a letra no alfabeto.
5. Exibe o resultado e acrescenta uma linha ao arquivo `resultado_criptografia.txt`.

A fórmula usada em cada posição `i` é:

```text
deslocamento = (SHIFT + termo_da_sequencia[i]) mod 26
letra_codificada = (letra_original + deslocamento) no alfabeto A-Z
```

O alfabeto é circular: depois de Z, a contagem volta para A. Letras maiúsculas e minúsculas são aceitas e preservam a caixa. Os termos são reduzidos módulo 26 para manter os cálculos pequenos; isso não altera o deslocamento aplicado às letras.

## Sequências disponíveis

- **PA:** começa em 1; o usuário informa a razão `r`. Exemplo com `r = 2`: `1, 3, 5, 7, ...`.
- **PG:** começa em 1; o usuário informa a razão `q`. Exemplo com `q = 2`: `1, 2, 4, 8, ...`.
- **Fibonacci:** `1, 1, 2, 3, 5, 8, ...`.
- **Primos:** `2, 3, 5, 7, 11, 13, ...`.

## Como compilar e executar

Com GCC instalado:

```bash
gcc -std=c11 -Wall -Wextra -pedantic criptografia.c -o criptografia
./criptografia
```

No Windows, após compilar com GCC/MinGW, execute `criptografia.exe`.

## Exemplo de conferência

Entrada:

```text
Palavra: coracao
SHIFT: 3
Sequencia: Fibonacci
```

Termos Fibonacci para as sete letras: `1, 1, 2, 3, 5, 8, 13`.
Somando o SHIFT 3, os deslocamentos são `4, 4, 5, 6, 8, 11, 16`.

Resultado conforme a fórmula descrita: **`gswgkle`**.

> Observação: o material de referência informa `fqvdjqb` para essa entrada, mas esse resultado não corresponde à regra nele apresentada (SHIFT + termo de Fibonacci). Este projeto segue a regra matemática escrita e explicita os termos usados, para facilitar a conferência.

## Arquivo de resultado

A cada execução válida, o programa acrescenta uma linha a `resultado_criptografia.txt`, por exemplo:

```text
Original: coracao | Codificada: gswgkle | SHIFT: 3 | Sequencia: Fibonacci | Letras: 7
```

O arquivo é criado na pasta em que o programa for executado. As execuções são acrescentadas ao final, preservando o histórico.

## Relação com a Taxonomia de Bloom

| Nível | Aplicação no projeto |
|---|---|
| Lembrar | Reconhecer o alfabeto, valores das letras e sequências. |
| Compreender | Explicar como SHIFT e os termos alteram cada letra. |
| Aplicar | Compilar e executar o programa em C. |
| Analisar | Comparar as palavras codificadas com diferentes sequências. |
| Avaliar | Discutir padrões e limitações da cifra. |
| Criar | Alterar a razão da PA/PG ou adicionar outra sequência. |

## Conceitos de programação utilizados

- funções e tipos enumerados;
- menu de opções e validação de entrada;
- vetores e strings;
- laços e condicionais;
- aritmética modular;
- leitura e gravação de arquivos com `fopen` e `fprintf`.

## Limitação importante

Esta é uma cifra **educacional**, útil para estudar algoritmos e padrões. Cifra de César e deslocamentos por sequência não protegem informações reais e não devem ser usados como criptografia de segurança.
