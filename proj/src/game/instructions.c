#include "instructions.h"
#include "../assets/font.h"
#include "../assets/game_pixmap.h"
#include "../peripherals/graphics/graphics.h"
#include "../peripherals/keyboard/keyboard.h"
#include "game.h"
#include "hangman.h"
#include "menu.h"
#include <lcom/lcf.h>
#include <stdio.h>

extern StateOptions state;

void draw_instructions_screen(void) {

  clear_screen();

  const char *instructions[] = {"Welcome to Hangman!\n\n",
                                "Rules:\n",
                                "- Guess the hidden word one letter at a time.\n",
                                "- Each incorrect guess draws a part of the hangman.\n",
                                "- You win if you guess the word before the hangman is complete.\n",
                                "- Use the arrow keys to navigate.\n",
                                "- Press ENTER to select.\n",
                                "Good luck!"};

  // 'Instructions' position
  int ix = (SCREEN_WIDTH - (strlen("Instructions") * 8 * 4)) / 2;
  int iy = 100;

  // button position
  int button_width = 200;
  int button_height = 50;
  int bx = (SCREEN_WIDTH - button_width) / 2;
  int by = SCREEN_HEIGHT - button_height - 80;

  // Center the "Back" text on the button
  const char *back_label = "Back";
  int label_width = strlen(back_label) * 8 * 2;
  int label_height = 8 * 2;
  int label_x = bx + (button_width - label_width) / 2;
  int label_y = by + (button_height - label_height) / 2;

  // instructions position
  int num_lines = sizeof(instructions) / sizeof(instructions[0]);
  int line_spacing = 20 * 2;
  int x = 100;
  int y = (SCREEN_HEIGHT - ((num_lines - 1) * line_spacing)) / 2;

  // Draw 'Instructions'
  draw_string("Instructions", ix, iy, BLUE_COLOR, 4);

  // Draw the instructions
  for (int i = 0; i < num_lines; i++) {
    draw_string(instructions[i], x, y + i * line_spacing, PINK_COLOR, 2);
  }

  // Draw the button rectangle
  vg_draw_rectangle(bx, by, button_width, button_height, BUTTON_COLOR);

  // Draw the 'Back' button
  draw_string(back_label, label_x, label_y, TEXT_COLOR, 2);

  if (scancode == KEY_ENTER) {
      state = MENU;
  }

  return;
}
