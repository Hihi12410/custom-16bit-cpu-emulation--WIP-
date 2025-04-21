#include "helper.h"
#include <stdio.h>

memory_image _vm_memory_init(unsigned long size)
{
    void * ptr = malloc(size);
    if (ptr == NULL) 
    {
        perror("Error making VM memory image!");
        exit(EXIT_FAILURE);
    }

    return (memory_image) {size, 0x0, ptr, 0};
}
vram _vm_vram_init_8bit(memory_image * mem, unsigned long x_size, unsigned long y_size, char def_col) 
{
    unsigned long vram_size = x_size * y_size;
    uint8_t * start = (mem->content+mem->size) - vram_size;
    
    if (vram_size > mem->size) {
        perror("Insufficient memory for VRAM!");
        exit(EXIT_FAILURE);
    }

    for (unsigned long i = 0; i < vram_size; ++i) 
    {
        start[i] = def_col;
    }

    mem->vmem_start = start;
    return (vram) 
    {
        x_size,
        y_size,
        8,
        start
    };
}

unsigned long _vm_load_binary(memory_image * mem, char * bin_path)
{
    FILE *src = fopen(bin_path, "rb");
    if (src == NULL) 
    {
        perror("\nError opening binary file!\n");
        return 0xFFFFFFFFFFFFFFFF;
    }

    fseek(src, 0, SEEK_END);
    unsigned long file_size = ftell(src);
    fseek(src, 0, SEEK_SET);

    if (file_size > (mem->vmem_start - mem->content))
    {
        printf("\nError file size is larger than memory capacity! File size : %lu Mem size : %lu \n", file_size, mem->vmem_start - mem->content);
        fclose(src);
        return 0xFFFFFFFFFFFFFFFF;
    }

    unsigned long nbytes = fread(mem->content, sizeof(char), file_size, src);
    if (nbytes != file_size)
    {
        printf("\nError failed to read the entire file!\n");
        fclose(src);
        return 0xFFFFFFFFFFFFFFFF;
    }

    fclose(src);
    return nbytes;
}


void _vm_error_dump_core(memory_image * mem, char * filename) 
{
    FILE * dest = fopen(filename, "wb");
    fwrite(mem->content, sizeof(char), mem->size, dest);
    fclose(dest);
}

void _vm_memory_free(memory_image * mem) 
{
    free(mem->content);
    mem->content = NULL;
    mem->size = 0;
    mem->vmem_start = 0;
}

unsigned short reg_val(char reg, reg_map * regmap) 
{
    if (reg == 0x0) 
    {
        return regmap->R0;
    }
    else if (reg == 0x1) 
    {
        return regmap->R1;
    }
    else if (reg == 0x2) 
    {
        return regmap->R2;
    }
    else if (reg == 0x3) 
    {
        return regmap->R3;
    }
    else if (reg == 0x4) 
    {
        return regmap->R4;
    }
    else if (reg == 0xA) 
    {
        return regmap->IN;
    }
    else if (reg == 0xB) 
    {
        return regmap->RF;
    }
    else 
    {
        return regmap->PC;
    }
}

void reg_sto(char reg, reg_map * regmap, uint16_t value) 
{
    if (reg == 0x0) 
    {
        regmap->R0 = value;
        return;
    }
    else if (reg == 0x1) 
    {
        regmap->R1 = value;
        return;
    }
    else if (reg == 0x2) 
    {
        regmap->R2 = value;
        return;
    }
    else if (reg == 0x3) 
    {
        regmap->R3 = value;
        return;
    }
    else if (reg == 0x4) 
    {
        regmap->R4 = value;
        return;
    }
    else if (reg == 0xA) 
    {
        regmap->IN = value;
        return;
    }
    else if (reg == 0xB) 
    {
        regmap->RF = value;
        return;
    }
    regmap->PC = value;
    return;
}

void split_dword(uint16_t dword, uint8_t * low, uint8_t * high)
{
    *low = dword & 0xFF;
    *high = (dword>>8) & 0xFF;
    return;
}

void combine_word(uint8_t low, uint8_t high, uint16_t  * dword)
{
    *dword = ((uint16_t)high<<8) | low;
    return;
}

int memsto_word(uint16_t address, uint8_t byte, memory_image * memory)
{
    if (address >= (memory->vmem_start - memory->content)) 
    {
        perror("\nOut of bounds memory write!\n");
        return -1;
    }
    ((uint8_t*)memory->content)[address] = byte;
    return 0;
}

int memsto_dword(uint16_t address, uint16_t byte, memory_image * memory)
{
    if (address+1 >= (memory->vmem_start - memory->content)) 
    {
        perror("\nOut of bounds memory write!\n");
        return -1;
    }

    uint8_t low;
    uint8_t high;
    
    split_dword(byte, &low, &high);

    ((uint8_t*)memory->content)[address] = low;
    ((uint8_t*)memory->content)[address+1] = high;

    return 0;
}

int memload_word(uint16_t address, uint8_t reg, memory_image * memory, reg_map * regm)
{
    if (address >= (memory->vmem_start - memory->content)) 
    {
        perror("\nOut of bounds memory read!\n");
        return -1;
    }

    reg_sto(reg, regm,(uint16_t)(memory->content[address]));
    
    return 0;
}   

int memload_dword(uint16_t address, uint8_t reg, memory_image * memory, reg_map * regm)
{
    if (address+1 >= (memory->vmem_start - memory->content)) 
    {
        perror("\nOut of bounds memory read!\n");
        return -1;
    }

    uint16_t dword;

    combine_word(memory->content[address],memory->content[address+1], &dword);
    reg_sto(reg, regm, dword);
    
    return 0;
}

int exec_word(reg_map * reg, memory_image * mem, cpu_state * state)
{
    printf("\nptr : %ld, val : %x", mem->ptr, mem->content[mem->ptr]);
    if (mem->ptr >= 100) 
    {
        return -1;
    } //End of memory
    
    if (state->inword == 0) 
    {
        state->sto[0] = mem->content[mem->ptr++];
        state->ptr = 1;

        printf("\n%x is the current opcode.", state->sto[0]);
        switch (state->sto[0]) {
            case OP_NOP: {
                state->read_left = 0;
                printf("\n%d", state->read_left);
                break;
            }
            case OP_ADD:{
                state->read_left = 2;
                printf("\n%d", state->read_left);
            }
            case OP_SUB : {
                state->read_left = 2;
            }
            case OP_LTR: {
                state->read_left = 3;
            }
            case OP_STR: {
                state->read_left = 2;
            }
            case OP_STM: {
                state->read_left = 3;
                printf("\n%d", state->read_left);
            }
            case OP_AND: {
                state->read_left = 2;
            }
            case OP_LOR: {
                state->read_left = 2;
            }
            case OP_LSH: {
                state->read_left = 3;
            }
            case OP_RSH: {
                state->read_left = 3;
            }
            case OP_NOT: {
                state->read_left = 1;
            }
            case OP_XOR: {
                state->read_left = 2;
            }
            case OP_JMP: 
            {
                state->read_left = 0;
            }
            case OP_JNZ: 
            {
                state->read_left = 1;
            }
            case OP_STL:{
                state->read_left = 3;
                printf("\n%d", state->read_left);
            }

            printf("\nop : %x , reads left: %d ", state->sto[0], state->read_left);
        }
    }

    if (state->read_left > 0) 
    {
        state->sto[state->ptr++] = ((char*)mem->content)[mem->ptr++];
        state->read_left = state->read_left-1;
        return 1;
    }else {
        state->inword = 0;
    }

    char op = state->sto[0];
    printf("\nop : %x", op);

    switch (op) {
        case OP_NOP: {
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_ADD:{
            reg->R0 = reg_val(state->sto[1], reg) + reg_val(state->sto[2], reg);
            printf("Adding: R%d + R%d = R0 -> %d + %d = %d\n", state->sto[1], state->sto[2], state->sto[0], state->sto[1], reg->R0);
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_SUB : {
            reg->R0 = reg_val(state->sto[1], reg) - reg_val(state->sto[2], reg);;
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_LTR: {
            char dest_reg = state->sto[1];
            unsigned char addr_low = (unsigned char)state->sto[2];
            unsigned char addr_high = (unsigned char)state->sto[3];
        
            uint16_t addr;
            combine_word(addr_low, addr_high, &addr);
            
            memload_dword(addr, dest_reg, mem, reg);
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_STR: {
            reg_sto( state->sto[1], reg, reg_val(state->sto[2], reg));
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_STM: {
            uint16_t base_val = reg_val(state->sto[1], reg);
            uint16_t ptr;
            combine_word(state->sto[2], state->sto[3], &ptr);
            memsto_dword(ptr, base_val, mem);
            printf("\nstored %u to %u\n", base_val, ptr);
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_AND: {
            uint16_t lhs = state->sto[1];
            uint16_t rhs = state->sto[2];
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_LOR: {
            uint16_t lhs = state->sto[1];
            uint16_t rhs = state->sto[2];
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_LSH: {
            
            uint16_t val = state->sto[1];
            uint16_t amount;
            combine_word(state->sto[2], state->sto[3], &amount);
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_RSH: {
            
            uint16_t val = state->sto[1];
            uint16_t amount;
            combine_word(state->sto[2], state->sto[3], &amount);
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_NOT: {
            
            uint16_t val = state->sto[1];
            reg->R0 = (uint16_t)(!val);
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_XOR: {
            
            uint16_t val = state->sto[1];
            uint16_t xor_val = state->sto[2];
            reg->R0 = (uint16_t)(val ^ xor_val);
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_JMP: 
        {
            mem->ptr = reg->PC;
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_JNZ: 
        {
            if (state->sto[1] != 0) mem->ptr = reg->PC;
            reg->PC = mem->ptr;
            return 0;
        }
        case OP_SPC: 
        {
            mem->ptr = reg->PC;
            return 0;
        }
        case OP_STL: 
        {
            uint16_t val;
            combine_word(state->sto[2], state->sto[3], &val);
            reg_sto(state->sto[1], reg, val);
            return 0;
        }
    }
    return -1; //Undefined op         
}
