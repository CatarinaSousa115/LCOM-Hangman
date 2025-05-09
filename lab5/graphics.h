#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lcom/lcf.h>
#include "VBE.h"

int (end_loop_ESC)();

int (vg_init_graphic)(uint16_t mode);

int (map_video_memory)(uint16_t mode);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int normalize_color(uint32_t color, uint32_t *new_color);

#endif

