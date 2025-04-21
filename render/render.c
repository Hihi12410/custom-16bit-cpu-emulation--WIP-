#include "render.h"

const uint32_t _grayscale_palette[256] = 
{
    #define SHADE(i) (uint32_t)((i << 24) | (i << 16) | (i << 8) | 0xFF)
    SHADE(0), SHADE(1), SHADE(2), SHADE(3), SHADE(4), SHADE(5), SHADE(6), SHADE(7),
    SHADE(8), SHADE(9), SHADE(10), SHADE(11), SHADE(12), SHADE(13), SHADE(14), SHADE(15),
    SHADE(16), SHADE(17), SHADE(18), SHADE(19), SHADE(20), SHADE(21), SHADE(22), SHADE(23),
    SHADE(24), SHADE(25), SHADE(26), SHADE(27), SHADE(28), SHADE(29), SHADE(30), SHADE(31),
    SHADE(32), SHADE(33), SHADE(34), SHADE(35), SHADE(36), SHADE(37), SHADE(38), SHADE(39),
    SHADE(40), SHADE(41), SHADE(42), SHADE(43), SHADE(44), SHADE(45), SHADE(46), SHADE(47),
    SHADE(48), SHADE(49), SHADE(50), SHADE(51), SHADE(52), SHADE(53), SHADE(54), SHADE(55),
    SHADE(56), SHADE(57), SHADE(58), SHADE(59), SHADE(60), SHADE(61), SHADE(62), SHADE(63),
    SHADE(64), SHADE(65), SHADE(66), SHADE(67), SHADE(68), SHADE(69), SHADE(70), SHADE(71),
    SHADE(72), SHADE(73), SHADE(74), SHADE(75), SHADE(76), SHADE(77), SHADE(78), SHADE(79),
    SHADE(80), SHADE(81), SHADE(82), SHADE(83), SHADE(84), SHADE(85), SHADE(86), SHADE(87),
    SHADE(88), SHADE(89), SHADE(90), SHADE(91), SHADE(92), SHADE(93), SHADE(94), SHADE(95),
    SHADE(96), SHADE(97), SHADE(98), SHADE(99), SHADE(100), SHADE(101), SHADE(102), SHADE(103),
    SHADE(104), SHADE(105), SHADE(106), SHADE(107), SHADE(108), SHADE(109), SHADE(110), SHADE(111),
    SHADE(112), SHADE(113), SHADE(114), SHADE(115), SHADE(116), SHADE(117), SHADE(118), SHADE(119),
    SHADE(120), SHADE(121), SHADE(122), SHADE(123), SHADE(124), SHADE(125), SHADE(126), SHADE(127),
    SHADE(128), SHADE(129), SHADE(130), SHADE(131), SHADE(132), SHADE(133), SHADE(134), SHADE(135),
    SHADE(136), SHADE(137), SHADE(138), SHADE(139), SHADE(140), SHADE(141), SHADE(142), SHADE(143),
    SHADE(144), SHADE(145), SHADE(146), SHADE(147), SHADE(148), SHADE(149), SHADE(150), SHADE(151),
    SHADE(152), SHADE(153), SHADE(154), SHADE(155), SHADE(156), SHADE(157), SHADE(158), SHADE(159),
    SHADE(160), SHADE(161), SHADE(162), SHADE(163), SHADE(164), SHADE(165), SHADE(166), SHADE(167),
    SHADE(168), SHADE(169), SHADE(170), SHADE(171), SHADE(172), SHADE(173), SHADE(174), SHADE(175),
    SHADE(176), SHADE(177), SHADE(178), SHADE(179), SHADE(180), SHADE(181), SHADE(182), SHADE(183),
    SHADE(184), SHADE(185), SHADE(186), SHADE(187), SHADE(188), SHADE(189), SHADE(190), SHADE(191),
    SHADE(192), SHADE(193), SHADE(194), SHADE(195), SHADE(196), SHADE(197), SHADE(198), SHADE(199),
    SHADE(200), SHADE(201), SHADE(202), SHADE(203), SHADE(204), SHADE(205), SHADE(206), SHADE(207),
    SHADE(208), SHADE(209), SHADE(210), SHADE(211), SHADE(212), SHADE(213), SHADE(214), SHADE(215),
    SHADE(216), SHADE(217), SHADE(218), SHADE(219), SHADE(220), SHADE(221), SHADE(222), SHADE(223),
    SHADE(224), SHADE(225), SHADE(226), SHADE(227), SHADE(228), SHADE(229), SHADE(230), SHADE(231),
    SHADE(232), SHADE(233), SHADE(234), SHADE(235), SHADE(236), SHADE(237), SHADE(238), SHADE(239),
    SHADE(240), SHADE(241), SHADE(242), SHADE(243), SHADE(244), SHADE(245), SHADE(246), SHADE(247),
    SHADE(248), SHADE(249), SHADE(250), SHADE(251), SHADE(252), SHADE(253), SHADE(254), SHADE(255)
    #undef SHADE
};
const color_table _grayscale_table = 
{
    256,
    1,
    (void*)_grayscale_palette
};
display init_video(unsigned long x_res,unsigned long y_res,unsigned long color_depth,color_table col_table, vram vmem)
{ 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Error initializing SDL! SDL_Error: %s\n", SDL_GetError());
        return (display) 
        {
            0,
            0,
            0,
            (color_table) 
            {
                0,
                0,
                NULL
            },
            (SDL_Renderer*) NULL,
            (SDL_Window*) NULL,
            (vram) {0, 0, 0, NULL}
        };
    };
    
    SDL_Window * window = SDL_CreateWindow("Custom CPU emulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_res, y_res, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) 
    {
        printf("Error initializing Window! SDL_Error: %s\n", SDL_GetError());
        return (display) 
        {
            0,
            0,
            0,
            (color_table) 
            {
                0,
                0,
                NULL
            },
            (SDL_Renderer*) NULL,
            (SDL_Window*) NULL,
            (vram) {0, 0, 0, NULL}
        };
    }
    SDL_RaiseWindow(window);


    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, x_res, y_res);
    
    if (renderer == NULL) 
    {
        printf("Error initializing renderer! SDL_Error: %s\n", SDL_GetError());
        return (display) 
        {
            0,
            0,
            0,
            (color_table) 
            {
                0,
                0,
                NULL
            },
            (SDL_Renderer*) NULL,
            (SDL_Window*) NULL,
            (vram) {0, 0, 0, NULL}
        };
    }

    return (display) 
    {
        x_res,
        y_res,
        color_depth,
        col_table,
        renderer,
        window,
        vmem
    };
}
void _render(display disp, vram mem) 
{
    SDL_RenderClear(disp.renderer);

    uint32_t *color_palette = (uint32_t*) disp.col_table.ptr;
    uint8_t *content = (uint8_t*) mem.ptr;

    for (unsigned long y = 0; y < mem.y_size; ++y) 
    {
        for (unsigned long x = 0; x < mem.x_size; ++x) 
        {
            uint8_t pixel_val = content[y * mem.x_size + x];
            Uint32 color = color_palette[pixel_val];

            Uint8 r = (color >> 24) & 0xFF;
            Uint8 g = (color >> 16) & 0xFF;
            Uint8 b = (color >> 8) & 0xFF;  

            SDL_SetRenderDrawColor(disp.renderer, r, g, b, 0xFF);
            SDL_RenderDrawPoint(disp.renderer, x, y);
        }
    }   

    SDL_RenderPresent(disp.renderer);
}

void display_cleanup(display disp) 
{
    SDL_DestroyRenderer(disp.renderer);
    SDL_DestroyWindow(disp.window);
    SDL_Quit();

    disp.renderer = (SDL_Renderer*)NULL;
    disp.window = (SDL_Window*)NULL;
    disp.x_res = 0;
    disp.y_res = 0;
    disp.color_depth = 0;
}