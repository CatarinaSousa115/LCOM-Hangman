#ifndef _MENU_H_
#define _MENU_H_
#include <stdint.h>
#include <stdbool.h>
extern bool gameRunning; // Variable to control the game loop

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR 0xFF99FF
#define TEXT_COLOR 0xFFFFFF
#define BUTTON_SPACING 20
#define HIGHLIGHT_COLOR 0x8A538A
#define KEY_UP_ARROW 0x48
#define KEY_DOWN_ARROW 0x50
#define KEY_ENTER 0x1C


typedef enum {
  MENU,
  MENU_PLAY,
  MENU_INSTRUCTIONS,
  MENU_EXIT
} MenuOptions;

void menu_init();
void draw_options();
void handle_menu_input(uint8_t scancode, int *selected_option);

#endif

