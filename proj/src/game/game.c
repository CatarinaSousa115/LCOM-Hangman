#include "game.h"
#include "menu.h"
#include "words.h"
#include "../assets/font.h"
#include "../peripherals/graphics/graphics.h"
#include "../peripherals/i8042.h"
#include <stdio.h>
#include <stdlib.h>

extern uint32_t timer_counter;
extern const char *guess_word;
extern int current_stage;

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


int handle_game_input(uint8_t scancode) {

  char letter = scancode_to_uppercase(scancode);

  //not a letter
  if (letter == 0) {
    return 1; 
  }

  int letter_index = letter - 'A'; //calculate the index of the letter in the array of booleans
    
   //letter already gessed
  if (guessed_letters[letter_index]) {
    return 1;
  }

  guessed_letters[letter_index] = true;
  bool correct_guess = false;
    
  //verify if the letter is in the word
  int word_size = strlen(guess_word);
  for (int i = 0; i < word_size; i++) {
    if (guess_word[i] == letter) {

      //if the letter is in the word we change the "_" in the displayed word to the letter gessed
      displayed_word[i] = letter;
      correct_guess = true;
    }
  }
    
  //incorrect guess
  if (!correct_guess) {
    current_stage++;  //the hangman goes to the next stage
  }
    
  return 0;
}


void draw_word_guesses() {

  int word_size = strlen(guess_word);

  draw_string(guess_word, 300, 200, 0XFFFFFF, 5);

  for (int i = 0; i < word_size; i++) {
    draw_string("_", 300 + (50 * i), 500, 0XFFFFFF, 5);
  }
}


