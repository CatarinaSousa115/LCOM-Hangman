#ifndef WORDS_H
#define WORDS_H

#include <lcom/lcf.h>
#include <stdbool.h> 

extern const char *guesswords[];
extern const int guesswords_count;
extern const char *guess_word;
extern bool guessed_letters[26];  
extern char displayed_word[];

#define MAX_WORD_LENGTH 20 //aux for tha max length we allow in a word

typedef struct {
    uint8_t scancode;
    char uppercase;
} KeyMap;

void generate_guessword();

char scancode_to_uppercase(uint8_t scancode);

#endif

