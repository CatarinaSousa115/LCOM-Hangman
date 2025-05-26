#include "menu.h"
#include "../assets/font.h"
#include "../peripherals/graphics/graphics.h"

void menu_init() {

  draw_options();

  return;
}

void draw_options() {
  int total_height = 3 * BUTTON_HEIGHT + 2 * BUTTON_SPACING;
  int y = (480 - total_height) / 2;

  // Draw "Start" button
  if (vg_draw_rectangle(total_height, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR) != 0) {
    printf("Failed to draw Start button\n");
  }
  if (draw_string("Start", total_height + 70, y + 15, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw Start text\n");
  }

  // Draw "Settings" button
  y += BUTTON_HEIGHT + BUTTON_SPACING;
  if (vg_draw_rectangle(total_height, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR) != 0) {
    printf("Failed to draw Settings button\n");
  }
  if (draw_string("Settings", total_height + 55, y + 15, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw Settings text\n");
  }

  // Draw "Exit" button
  y += BUTTON_HEIGHT + BUTTON_SPACING;
  if (vg_draw_rectangle(total_height, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR) != 0) {
    printf("Failed to draw Exit button\n");
  }
  if (draw_string("Exit", total_height + 75, y + 15, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw Exit text\n");
  }
}
