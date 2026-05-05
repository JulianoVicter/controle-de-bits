# Funcionamento do Código

Este código demonstra como empacotar 4 valores de 6 bits em 3 bytes e depois recuperar os valores originais.

---

## Bibliotecas

```c
#include <stdio.h>
#include <stdint.h>
```

### `stdio.h`

Usada para imprimir informações no terminal com `printf`.

### `stdint.h`

Usada para trabalhar com tipos inteiros de tamanho fixo:

```c
uint8_t
uint32_t
```

---

## `print_binario_8`

```c
void print_binario_8(uint8_t n)
```

Essa função imprime um número de 8 bits em formato binário.

Ela percorre os bits da esquerda para a direita, começando pelo bit mais significativo:

```c
for (int i = 7; i >= 0; i--)
```

Para descobrir o valor de cada bit, usa:

```c
(n >> i) & 1
```

O trecho:

```c
n >> i
```

desloca o bit desejado até a última posição.

Depois:

```c
& 1
```

pega apenas esse último bit.

A função imprime `0` ou `1` para cada posição.

---

## `print_binario_24`

```c
void print_binario_24(uint32_t n)
```

Essa função imprime os 24 bits usados no pacote.

Ela funciona de forma parecida com `print_binario_8`, mas percorre os bits de `23` até `0`:

```c
for (int i = 23; i >= 0; i--)
```

Também usa:

```c
(n >> i) & 1
```

para pegar cada bit individualmente.

---

## `main`

```c
int main(void)
```

A função `main` executa o programa.

Nela são definidos os valores que serão empacotados:

```c
uint8_t v0 = 50;
uint8_t v1 = 12;
uint8_t v2 = 63;
uint8_t v3 = 7;
```

Cada valor precisa estar entre `0` e `63`.

---

## Validação dos valores

```c
if (v0 > 63 || v1 > 63 || v2 > 63 || v3 > 63)
```

Essa verificação impede que valores maiores que 6 bits sejam empacotados.

Se algum valor for maior que `63`, o programa mostra erro e encerra.

---

## Criação do pacote

```c
uint32_t pacote = 0;
```

Essa variável armazena temporariamente os 24 bits formados pelos 4 valores.

---

## Empacotamento

```c
pacote |= ((uint32_t)v0 << 18);
pacote |= ((uint32_t)v1 << 12);
pacote |= ((uint32_t)v2 << 6);
pacote |= ((uint32_t)v3);
```

Cada valor é colocado em uma região específica do pacote.

A organização fica assim:

```txt
bits 23 até 18 -> v0
bits 17 até 12 -> v1
bits 11 até  6 -> v2
bits  5 até  0 -> v3
```

### `v0`

```c
((uint32_t)v0 << 18)
```

Move `v0` para os bits `23` até `18`.

### `v1`

```c
((uint32_t)v1 << 12)
```

Move `v1` para os bits `17` até `12`.

### `v2`

```c
((uint32_t)v2 << 6)
```

Move `v2` para os bits `11` até `6`.

### `v3`

```c
((uint32_t)v3)
```

Fica nos bits `5` até `0`.

O operador:

```c
|
```

junta os bits dentro da mesma variável.

---

## Separação em bytes

Depois de montar o pacote de 24 bits, ele é separado em 3 bytes:

```c
uint8_t byte0 = (pacote >> 16) & 0xFF;
uint8_t byte1 = (pacote >> 8) & 0xFF;
uint8_t byte2 = pacote & 0xFF;
```

### `byte0`

```c
(pacote >> 16) & 0xFF
```

Pega os 8 bits mais à esquerda do pacote.

### `byte1`

```c
(pacote >> 8) & 0xFF
```

Pega os 8 bits do meio.

### `byte2`

```c
pacote & 0xFF
```

Pega os 8 bits mais à direita.

A máscara:

```c
0xFF
```

mantém apenas 8 bits.

---

## Reconstrução do pacote

```c
uint32_t recebido = 0;

recebido |= ((uint32_t)byte0 << 16);
recebido |= ((uint32_t)byte1 << 8);
recebido |= ((uint32_t)byte2);
```

Aqui os 3 bytes são juntados novamente em uma variável de 24 bits.

### `byte0`

```c
((uint32_t)byte0 << 16)
```

Volta para os bits mais altos.

### `byte1`

```c
((uint32_t)byte1 << 8)
```

Volta para os bits intermediários.

### `byte2`

```c
((uint32_t)byte2)
```

Fica nos bits mais baixos.

---

## Recuperação dos valores

```c
uint8_t r0 = (recebido >> 18) & 0x3F;
uint8_t r1 = (recebido >> 12) & 0x3F;
uint8_t r2 = (recebido >> 6) & 0x3F;
uint8_t r3 = recebido & 0x3F;
```

Cada valor é recuperado usando deslocamento e máscara.

A máscara:

```c
0x3F
```

mantém apenas 6 bits.

---

### Recuperando `r0`

```c
uint8_t r0 = (recebido >> 18) & 0x3F;
```

Move os bits de `v0` para a direita e mantém apenas os 6 bits finais.

---

### Recuperando `r1`

```c
uint8_t r1 = (recebido >> 12) & 0x3F;
```

Move os bits de `v1` para a direita e mantém apenas os 6 bits finais.

---

### Recuperando `r2`

```c
uint8_t r2 = (recebido >> 6) & 0x3F;
```

Move os bits de `v2` para a direita e mantém apenas os 6 bits finais.

---

### Recuperando `r3`

```c
uint8_t r3 = recebido & 0x3F;
```

Mantém diretamente os últimos 6 bits, onde está `v3`.

---

## Operadores usados

### `<<`

Desloca bits para a esquerda.

Usado para colocar um valor em uma posição específica do pacote.

---

### `>>`

Desloca bits para a direita.

Usado para trazer um valor de volta para os bits finais antes de aplicar a máscara.

---

### `|`

Operador OR bit a bit.

Usado para juntar valores no mesmo pacote.

---

### `&`

Operador AND bit a bit.

Usado para isolar bits específicos.

---

## Máscaras usadas

### `0xFF`

```txt
11111111
```

Mantém 8 bits.

Usada para extrair bytes.

---

### `0x3F`

```txt
00111111
```

Mantém 6 bits.

Usada para extrair cada valor original.

---

## Fluxo do programa

```txt
1. Define os valores originais
2. Verifica se todos estão entre 0 e 63
3. Junta os 4 valores em um pacote de 24 bits
4. Divide o pacote em 3 bytes
5. Junta os 3 bytes novamente
6. Extrai os 4 valores originais
7. Imprime os resultados
```
