#include "menu.h"
#include "../assets/font.h"
#include "../assets/game_pixmap.h"
#include "../peripherals/graphics/graphics.h"
#include "../peripherals/keyboard/keyboard.h"
#include "hangman.h"
#include <lcom/lcf.h>
#include <stdio.h>

void draw_options(int selected_option) {
  // Define the labels for the buttons
  const char *labels[] = {"Start", "Settings", "Exit"};
  const int num_labels = 3;

  // Calculate the total height of all buttons and spacings
  int total_height = (num_labels * BUTTON_HEIGHT) + ((num_labels - 1) * BUTTON_SPACING);

  // Calculate the vertical center of the screen
  int y = (SCREEN_HEIGHT - total_height) / 2;

  // Loop through the labels and draw each button
  for (int i = 0; i < num_labels; i++) {
    // Calculate the horizontal center of the screen for each button
    int x = (SCREEN_WIDTH - BUTTON_WIDTH) / 2;

    // Highlight the selected option
    uint32_t color = (i == selected_option) ? HIGHLIGHT_COLOR : BUTTON_COLOR;

    // Draw the button rectangle
    if (vg_draw_rectangle(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, color) != 0) {
      printf("Failed to draw %s button\n", labels[i]);
    }

    // Calculate label width and height based on text size (3x scale)
    int label_width = strlen(labels[i]) * 8 * 3; // Each character is 8 pixels wide, scaled by 3
    int label_height = 8 * 3;                    // Each character is 8 pixels tall, scaled by 3

    // Center the label inside the button
    int label_x = x + (BUTTON_WIDTH - label_width) / 2;
    int label_y = y + (BUTTON_HEIGHT - label_height) / 2;

    // Draw the button label
    if (draw_string(labels[i], label_x, label_y, TEXT_COLOR, 3) != 0) {
      printf("Failed to draw %s text\n", labels[i]);
    }

    // Move to the next button position
    y += BUTTON_HEIGHT + BUTTON_SPACING;
  }
}

void handle_menu_input(uint8_t scancode, int *selected_option) {

  switch (scancode) {
    case ESC_BREAKCODE:
      gameRunning = false; // Exit the game loop
      break;

    case KEY_UP_ARROW:
      // Move up in the menu
      if (*selected_option > 0) {
        (*selected_option)--;
      }
      break;

    case KEY_DOWN_ARROW:
      // Move down in the menu
      if (*selected_option < 2) { // Assuming 3 options
        (*selected_option)++;
      }
      break;

    case KEY_ENTER:
      // Handle selection of the current option
      switch (*selected_option) {
        case 0:
          printf("Start game selected\n");
          // Start the game logic here
          break;
        case 1:
          printf("Settings selected\n");
          // Open settings logic here
          break;
        case 2:
          gameRunning = false; // Exit the game
          break;
      }
      break;

    default:
      // Handle other keys if necessary
      break;
  }
}
