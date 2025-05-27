#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

void draw_guessword();

void gameCountdown(int remaining_time);

void draw_game_over_screen();

void handle_game_input(uint8_t scancode);

#endif 
