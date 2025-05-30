#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "../../game/menu.h"
#include "../i8042.h"
#include "../k_controller.h"
#include <lcom/lcf.h>

int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
void(mouse_ih)();
void(buffer_mouse_bytes)();
int(mouse_write_cmd)(uint8_t cmd);
int init_mouse_pointer();
void save_background(uint16_t mouse_x, uint16_t mouse_y);
void restore_background(uint16_t mouse_x, uint16_t mouse_y);
void update_mouse_position_logic();
void update_mouse_position_with_double_buffering();
void draw_mouse_pointer_with_double_buffering();
void draw_mouse_pointer();
void process_mouse_clicks();
void init_mouse_background(size_t bytes_per_pixel);

#endif
