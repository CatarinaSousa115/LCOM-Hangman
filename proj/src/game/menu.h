#ifndef _MENU_H_
#define _MENU_H_
#include <stdint.h>
#include <stdbool.h>
#include "../peripherals/i8042.h"
extern bool gameRunning; // Variable to control the game loop

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR 15
#define TEXT_COLOR 29
#define HIGHLIGHT_COLOR 47
#define BUTTON_SPACING 20


void menu_init();
void draw_options();
void handle_menu_input(uint8_t scancode, int *selected_option);

#endif

