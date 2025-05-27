#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <lcom/lcf.h>
#include "../game/menu.h"
#include "../game/hangman.h"
#include "../game/game.h"
#include "../peripherals/graphics/graphics.h"
#include "../game/words.h"
#include "../assets/font.h"
#include "../assets/game_pixmap.h"

typedef enum {
  MENU,
  PLAY,
  INSTRUCTIONS,
  EXIT
} StateOptions;

extern uint32_t timer_counter;
extern StateOptions state;
extern int selected_option;
extern int remaining_time;
extern int current_stage;
extern bool redraw_needed;

void handle_game_state();

void reset_game_state();

#endif

