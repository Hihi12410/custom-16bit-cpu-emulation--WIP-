#include <stdint.h>
#include <stdio.h>

typedef enum {

    OP_NOP = 0x0,

    OP_ADD = 0x1,
    OP_SUB = 0x2,

    OP_LTR = 0x3,
    OP_STR = 0x4,
    OP_STM = 0x5,

    OP_AND = 0x6,
    OP_LOR = 0x7,
    OP_LSH = 0x8,
    OP_RSH = 0x9,
    OP_NOT = 0xA,
    OP_XOR = 0xB,

    OP_JMP = 0xC,
    OP_JNZ = 0xD,
    OP_SPC = 0xE,
    OP_STL = 0xF
} opcode;

typedef enum 
{
    REG_R0 = 0x0,
    REG_R1 = 0x1,
    REG_R2 = 0x2,
    REG_R3 = 0x3,
    REG_R4 = 0x4,
    REG_PC = 0xC,
    REG_RF = 0xA,
    REG_IN = 0xB
} reg;



int main() 
{
    uint8_t bytes[] = 
    {
        OP_NOP,
        OP_STL, REG_R1, 0xA,
        OP_STL, REG_R2, 0x2,
        OP_ADD, REG_R1, REG_R2,
        OP_STM,REG_R0,0x2,0x3,
        OP_NOP
    };
    FILE * f = fopen("out.bin", "wb");
    fwrite(bytes, sizeof(uint8_t), 14, f);
    fclose(f);
    return 0;

}

0xA + 0x2 = C