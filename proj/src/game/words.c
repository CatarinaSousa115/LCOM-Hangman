#include "words.h"
#include "../assets/font.h"
#include "../peripherals/i8042.h"

KeyMap key_map[26] = {
  {0x1E, 'A'},
  {0x30, 'B'},
  {0x2E, 'C'},
  {0x20, 'D'},
  {0x12, 'E'},
  {0x21, 'F'},
  {0x22, 'G'},
  {0x23, 'H'},
  {0x17, 'I'},
  {0x24, 'J'},
  {0x25, 'K'},
  {0x26, 'L'},
  {0x32, 'M'},
  {0x31, 'N'},
  {0x18, 'O'},
  {0x19, 'P'},
  {0x10, 'Q'},
  {0x13, 'R'},
  {0x1F, 'S'},
  {0x14, 'T'},
  {0x16, 'U'},
  {0x2F, 'V'},
  {0x11, 'W'},
  {0x2D, 'X'},
  {0x15, 'Y'},
  {0x2C, 'Z'},
};

//max word size is 10 (now)
const char *guesswords[] = {
    "HANGMAN",
    "COMPUTER",
    "PROGRAMMING",
    "KEYBOARD",
    "MOUSE",
    "MONITOR",      
    "SOFTWARE",     
    "HARDWARE",     
    "PROCESSOR", 
    "DATABASE",    
    "NETWORK",     
    "ALGORITHM",    
    "FUNCTION",     
    "COMPILER",    
    "TERMINAL",     
    "GRAPHICS",     
    "INTERNET",     
    "ROUTER",      
    "WEBCAM"
};

const int guesswords_count = sizeof(guesswords) / sizeof(guesswords[0]);
const char *guess_word;

//array of booleans to represent if the letter was already gessed
bool guessed_letters[26] = {false};
char displayed_word[MAX_WORD_LENGTH] = {0}; //aux so we can track the "word state"


void generate_guessword() {
  //generate a random index to select a word
  int random_index = rand() % guesswords_count;
  guess_word = guesswords[random_index];
   
  //initialize the word state with "_"
  int word_size = strlen(guess_word);
  for (int i = 0; i < word_size; i++) {
    displayed_word[i] = '_';
  }
  
  //the last index of the gess_word is \0 so we can know were the word end
  displayed_word[strlen(guess_word)] = '\0';
}


char scancode_to_letter(uint8_t scancode) {
  
  for (int i = 0; i < 26; i++) {  
    
    if (key_map[i].scancode == scancode) {
      return key_map[i].letter;
    }
  }
  
  return 0;  
}

