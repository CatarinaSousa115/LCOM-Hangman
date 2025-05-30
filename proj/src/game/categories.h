#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <lcom/lcf.h>

#define NUM_CATEGORIES 4

void draw_categories(int selected_option);

void handle_categories_input(uint8_t scancode, int *selected_option);

#endif

