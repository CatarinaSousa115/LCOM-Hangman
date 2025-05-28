#include "game.h"
#include "menu.h"
#include "words.h"
#include "../assets/font.h"
#include "../peripherals/graphics/graphics.h"
#include "../peripherals/i8042.h"
#include <stdio.h>
#include <stdlib.h>

extern uint32_t timer_counter;



void gameCountdown(int remaining_time) {
  
  int unit_number = remaining_time % 10; 
  int tens_number = (remaining_time/ 10) % 10;

  vg_draw_rectangle(SCREEN_WIDTH - 200, 50, 100, 80, 0x000000); 

  if (tens_number > 0) {
    
    draw_letter('0' + tens_number, SCREEN_WIDTH - 200, 60, 0xFFF000, 4);
    draw_letter('0' + unit_number, SCREEN_WIDTH - 150, 60, 0xFFF000, 4);
  }

  else {
    draw_letter('0' + unit_number, SCREEN_WIDTH - 150, 60, 0xFFF000, 4);
  }
}


void draw_game_over_screen() {

  vg_draw_rectangle(0, 0, SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, 0x000000); 
  draw_string("Time's up!", 500, 400, TEXT_COLOR, 3);
}


void handle_game_input(uint8_t scancode) {

  
  if (scancode == SCANCODE_A) {
    draw_string("Pressed A!", 500, 400, TEXT_COLOR, 3);
  }

  if (scancode == SCANCODE_B) {
    draw_string("Pressed B!", 500, 400, TEXT_COLOR, 3);
  }
}
