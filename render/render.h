#pragma once
#include "SDL2/SDL.h"
#include "../utils/helper.h"
#include <SDL2/SDL_render.h>

typedef struct 
{
    unsigned long len;
    unsigned long item_size;
    void * ptr;

} color_table;

typedef struct 
{
    unsigned long x_res;
    unsigned long y_res;
    unsigned long color_depth;
    color_table col_table;
    SDL_Renderer * renderer;
    SDL_Window * window;
    vram vmem;
    SDL_Texture * tex;
    uint32_t * pixel_data;

}display ;
#define ERROR_DISP (display) { 0, 0, 0, (color_table) { 0, 0, NULL }, (SDL_Renderer*) NULL, (SDL_Window*) NULL, (vram) {0, 0, 0, NULL}, NULL, NULL }


display init_video(unsigned long x_res,unsigned long y_res,unsigned long color_depth,color_table col_table, vram vmem);

void _render(display * disp, vram mem);

void display_cleanup(display disp);



//Pre-def color tables


extern const uint32_t _grayscale_palette[256];
extern const color_table _grayscale_table;
