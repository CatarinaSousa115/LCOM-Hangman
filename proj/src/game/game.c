#include "game.h"
#include "menu.h"
#include "words.h"
#include "hangman.h"
#include "../assets/font.h"
#include "../peripherals/graphics/graphics.h"
#include "../peripherals/i8042.h"
#include <stdio.h>
#include <stdlib.h>

extern uint32_t timer_counter;
extern int remaining_time;
extern const char *guess_word;
extern int current_stage;
extern char displayed_word[];

void gameCountdown(int remaining_time) {

  int unit_number = remaining_time % 10; 
  int tens_number = (remaining_time/ 10) % 10;

  vg_draw_rectangle(SCREEN_WIDTH - 200, 50, 100, 80, 0x000000); 

  if (tens_number > 0) {
    
    draw_letter('0' + tens_number, SCREEN_WIDTH - 200, 60, 3, 4);
    draw_letter('0' + unit_number, SCREEN_WIDTH - 150, 60, 3, 4);
  }

  else {
    draw_letter('0' + unit_number, SCREEN_WIDTH - 150, 60, 36, 4);
  }
}

bool check_win() {
  //check if the player has won
  int word_size = strlen(guess_word);
  
  for (int i = 0; i < word_size; i++) {
    if (displayed_word[i] == '_') {
      return false;
    }
  }

  return true;
}


void draw_game_over_screen() {

  clear_screen();
  const char *output_text; 

  if (check_win()) {
    output_text = "You win!";
  } 
  
  else {
    if(remaining_time == 0){
    output_text = "Time's up!";
    }
    else {
      output_text = "You lose!";
    }
  }

  draw_string(output_text, (SCREEN_WIDTH - strlen(output_text) * 8 * 3) / 2, SCREEN_HEIGHT / 2, TEXT_COLOR, 3);
}


int handle_game_input(uint8_t scancode) {

  char letter = scancode_to_letter(scancode);

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
    
  remaining_time = TIME_LIMIT; 
  return 0;
}

//need te be changed
void draw_word_guesses() {

  int word_size = strlen(guess_word);
  int start_x = 650;

  draw_string(guess_word, 650, 200, 63, 4);

  for (int i = 0; i < word_size; i++) {
    char letter = displayed_word[i];

    draw_string(&letter, start_x + (50 * i), 500, 63, 4);
  }
}




