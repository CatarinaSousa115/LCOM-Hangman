#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "VBE.h"

int vg_init_graphic(uint16_t mode);

int init_framebuffer(uint16_t mode);

int this_vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

int this_vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int this_vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);



#endif



