#include "game.h"
#include "menu.h"
#include "words.h"
#include "../assets/font.h"
#include <stdio.h>
#include <stdlib.h>





void draw_guessword() {
  int x = 50;
  int y = 100;

  int random_index = rand() % guesswords_count;
  const char *guess_word = guesswords[random_index];

  if(draw_string(guess_word, x, y, TEXT_COLOR, 3) != 0) {
    printf("Failed to draw guess word\n");
  }
}
