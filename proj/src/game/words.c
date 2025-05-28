#include "words.h"
#include "../assets/font.h"
#include <lcom/lcf.h>

const char *guesswords[] = {
    "HANGMAN",
    "COMPUTER",
    "PROGRAMMING",
    "KEYBOARD",
    "MOUSE"
};

const int guesswords_count = sizeof(guesswords) / sizeof(guesswords[0]);
const char *guess_word;

void generate_guessword() {
  // Generate a random index to select a word
  int random_index = rand() % guesswords_count;
  guess_word = guesswords[random_index];
   
}




