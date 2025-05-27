#ifndef GAME_H
#define GAME_H

typedef enum {
  MENU,
  PLAY,
  INSTRUCTIONS
} StateOptions;


void draw_guessword();

void gameCountdown(int remaining_time);

#endif 
