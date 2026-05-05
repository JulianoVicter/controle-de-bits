#include <stdio.h>
#include <stdint.h>

void print_binario_8(uint8_t n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

void print_binario_24(uint32_t n) {
    for (int i = 23; i >= 0; i--) {
        printf("%d", (n >> i) & 1);

        if (i % 6 == 0) {
            printf(" ");
        }
    }
}

int main(void) {
    uint8_t v0 = 50;
    uint8_t v1 = 12;
    uint8_t v2 = 63;
    uint8_t v3 = 7;

    if (v0 > 63 || v1 > 63 || v2 > 63 || v3 > 63) {
        printf("Erro: todos os valores precisam estar entre 0 e 63.\n");
        return 1;
    }
    uint32_t pacote = 0;

    pacote |= ((uint32_t)v0 << 18);
    pacote |= ((uint32_t)v1 << 12);
    pacote |= ((uint32_t)v2 << 6);
    pacote |= ((uint32_t)v3);

    printf("Valores originais:\n");
    printf("v0 = %u\n", v0);
    printf("v1 = %u\n", v1);
    printf("v2 = %u\n", v2);
    printf("v3 = %u\n", v3);

    printf("\nPacote de 24 bits:\n");
    print_binario_24(pacote);
    printf("\n");

 //concatenar os 4 valores em um único inteiro de 24 bits, onde cada valor ocupa 6 bits.
    uint8_t byte0 = (pacote >> 16) & 0xFF;
    uint8_t byte1 = (pacote >> 8) & 0xFF;
    uint8_t byte2 = pacote & 0xFF;

    printf("\nBytes gerados:\n");

    printf("byte0 = ");
    print_binario_8(byte0);
    printf(" = %u\n", byte0);

    printf("byte1 = ");
    print_binario_8(byte1);
    printf(" = %u\n", byte1);

    printf("byte2 = ");
    print_binario_8(byte2);
    printf(" = %u\n", byte2);
    printf("os 3 juntos: \n");
    print_binario_8(byte0);
    print_binario_8(byte1);
    print_binario_8(byte2);

// decodificar os 3 bytes para recuperar os valores originais
    uint32_t recebido = 0;

    recebido |= ((uint32_t)byte0 << 16);
    recebido |= ((uint32_t)byte1 << 8);
    recebido |= ((uint32_t)byte2);

    uint8_t r0 = (recebido >> 18) & 0x3F;
    uint8_t r1 = (recebido >> 12) & 0x3F;
    uint8_t r2 = (recebido >> 6) & 0x3F;
    uint8_t r3 = recebido & 0x3F;

    printf("\nValores recuperados:\n");
    printf("r0 = %u\n", r0);
    printf("r1 = %u\n", r1);
    printf("r2 = %u\n", r2);
    printf("r3 = %u\n", r3);

    return 0;
}