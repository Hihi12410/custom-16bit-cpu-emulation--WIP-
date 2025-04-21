#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEMORY_SIZE 65536
#define VMEM_SIZE 32768

#define RES_X 256
#define RES_Y 128

#define CLOCK_SPEED 500//hz

typedef struct 
{
    unsigned long size;
    uint8_t * vmem_start;
    uint8_t * content;
    unsigned long ptr;

} memory_image;

typedef struct
{
    unsigned long x_size;
    unsigned long y_size;
    unsigned long cell_size;
    void * ptr;
} vram;

typedef struct
{
    uint16_t R0;  //0x0
    uint16_t R1;  //0x1
    uint16_t R2;  //0x2
    uint16_t R3;  //0x3
    uint16_t R4;  //0x4

    uint16_t IN;   //0xA
    uint16_t RF;   //0xB
    uint16_t PC;   //0xC

} reg_map;

typedef struct 
{
    unsigned char sto[256];
    unsigned char ptr;
    unsigned char read_left;
    unsigned char inword;

} cpu_state ;

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


//Memory
memory_image _vm_memory_init(unsigned long size);

vram _vm_vram_init_8bit(memory_image * mem, unsigned long x_size, unsigned long y_size, char def_col);

unsigned long _vm_load_binary(memory_image * mem, char * bin_path);

void _vm_error_dump_core(memory_image * mem, char * filename);

void _vm_memory_free(memory_image * mem);

unsigned short reg_val(char reg, reg_map * regmap);
void reg_sto(char reg, reg_map * regmap,unsigned short value);

void split_dword(uint16_t dword, uint8_t * low, uint8_t * high);
void combine_word(uint8_t low, uint8_t high, uint16_t  * dword);

int memsto_word(uint16_t address, uint8_t byte, memory_image * memory);
int memsto_dword(uint16_t address, uint16_t byte, memory_image * memory);

int memload_word(uint16_t address, uint8_t reg, memory_image * memory, reg_map * regm);
int memload_dword(uint16_t address, uint8_t reg, memory_image * memory, reg_map * regm);

int exec_word(reg_map * reg, memory_image * mem, cpu_state * state);