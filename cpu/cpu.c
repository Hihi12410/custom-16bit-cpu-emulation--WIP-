#include "../utils/helper.h"
#include "../render/render.h"
#include <stdio.h>


int main(int argc, char ** argv) 
{
    //Mem & video init
    memory_image mem = _vm_memory_init(MEMORY_SIZE);    
    vram vmem = _vm_vram_init_8bit(&mem, RES_X, RES_Y, 0xFF);
    display disp = init_video(RES_X, RES_Y, 1, _grayscale_table, vmem);
    
    //Loading executable
    unsigned long bytes_read = _vm_load_binary(&mem, argv[1]);
    reg_map reg = (reg_map) 
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
    cpu_state state = (cpu_state) { {}, 0, 0, 0};

    if (bytes_read == 0xFFFFFFFFFFFFFFFF) {
        printf("\nFailed to load binary file.\n");
    } else {
        printf("\nLoaded %lu bytes successfully.\n", bytes_read);
    }

    SDL_Event e;
    int running = 0;
    int ret = 0;
    while (running >= 0 && ret >= 0) 
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) 
            {
                running = -1;
            }
        }

        ret = exec_word(&reg, &mem, &state);
    }

    _vm_error_dump_core(&mem, argv[2]);
    display_cleanup(disp);
    _vm_memory_free(&mem);
    return 0;
}