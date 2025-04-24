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



int main(int argc, char ** argv) 
{
    uint16_t color = 0xFFFF;
    uint16_t vmem_addr = 0x8000;
    uint16_t one = 2;
    uint16_t mem_size = 65533;
    uint16_t loop_start = 24;

    uint8_t bytes[] = 
    {
        OP_STL, REG_R1, 0, 0,                                                  //Reset counter
        OP_STL, REG_R2, color & 0xFF, color >> 8,                              //Store color
        OP_STL, REG_R3, one & 0xFF,one >> 8,                                 //Inc reg
        OP_STL, REG_R4, vmem_addr & 0xFF, vmem_addr >> 8,                  //vmem start addr
        OP_STL, REG_RF, mem_size & 0xFF, mem_size >> 8,                    //Max mem size
        OP_STL, REG_IN, loop_start & 0xFF, loop_start>>8,                  //Set loop start point

        OP_STM, REG_R4, REG_R4,                                                 //Set pixel color
        OP_ADD, REG_R3, REG_R4,                                                 //Imcrement pixel  
        OP_STR, REG_R4, REG_R0,                                                 //Store pointer to next pixel
        OP_SUB, REG_R4, REG_RF,
        OP_NOT, REG_R0,
        OP_JNZ, REG_R0,
        
        OP_SPC, REG_IN,
        OP_JNZ, REG_R0

    };

    FILE * f = fopen(argv[1], "wb");
    fwrite(bytes, sizeof(uint8_t), sizeof(bytes), f);
    fclose(f);
    return 0;

}