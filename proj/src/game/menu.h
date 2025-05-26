#ifndef _MENU_H_
#define _MENU_H_

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_COLOR 0xFF99FF
#define TEXT_COLOR 0xFFFFFF
#define BUTTON_SPACING 20

typedef enum {
  MENU,
  MENU_PLAY,
  MENU_INSTRUCTIONS,
  MENU_EXIT
} MenuOptions;

void menu_init();
void draw_options();

#endif

