#include "menu.h"
#include "../assets/font.h"
#include "../peripherals/graphics/graphics.h"
#include "hangman.h"
#include "../assets/game_pixmap.h" 
#include <lcom/lcf.h>
#include <stdio.h>

void menu_init() {

  
 /* if (draw_hangman(100, 50,0) != 0) {
  printf("Error drawing hangman.\n");}*/

  draw_options();

  return;
}

void draw_options() {
  // Calculate the horizontal center of the screen
  int x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;

  // Calculate the total height of all buttons and spacings
  int total_height = (3 * BUTTON_HEIGHT) + (2 * BUTTON_SPACING);

  // Calculate the vertical center of the screen
  int y = (SCREEN_HEIGHT - total_height) / 2;

  // Draw "Start" button
  if (vg_draw_rectangle(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR) != 0) {
    printf("Failed to draw Start button\n");
  }
  if (draw_string("Start", x + (BUTTON_WIDTH - (5 * 3 * 8)) / 2, y + (BUTTON_HEIGHT - (3 * 8)) / 2, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw Start text\n");
  }

  // Draw "Settings" button
  y += BUTTON_HEIGHT + BUTTON_SPACING;
  if (vg_draw_rectangle(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR) != 0) {
    printf("Failed to draw Settings button\n");
  }
  if (draw_string("Settings", x + (BUTTON_WIDTH - (8 * 3 * 8)) / 2, y + (BUTTON_HEIGHT - (3 * 8)) / 2, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw Settings text\n");
  }

  // Draw "Exit" button
  y += BUTTON_HEIGHT + BUTTON_SPACING;
  if (vg_draw_rectangle(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR) != 0) {
    printf("Failed to draw Exit button\n");
  }
  if (draw_string("Exit", x + (BUTTON_WIDTH - (4 * 3 * 8)) / 2, y + (BUTTON_HEIGHT - (3 * 8)) / 2, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw Exit text\n");
  }
}


