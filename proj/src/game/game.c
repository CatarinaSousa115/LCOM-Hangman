#include "game.h"
#include "menu.h"
#include "words.h"
#include "../assets/font.h"
#include <stdio.h>
#include <stdlib.h>

extern uint32_t timer_counter;

void draw_guessword() {
  int x = 50;
  int y = 100;

  int random_index = rand() % guesswords_count;
  const char *guess_word = guesswords[random_index];

  if(draw_string(guess_word, x, y, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw guess word\n");
  }
}

void gameCountdown(int remaining_time) {
  
  int unit_number = remaining_time % 10; 
  int tens_number = (remaining_time/ 10) % 10;

  if (tens_number > 0) {
    
    draw_letter('0' + tens_number, 40, 40, 0xFFF000, 4);
    draw_letter('0' + unit_number, 90, 40, 0xFFF000, 4);
  }

  else {
    draw_letter('0' + unit_number, 90, 40, 0xFFF000, 4);
  }
}
