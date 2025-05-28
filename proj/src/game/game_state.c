#include "game_state.h"

int current_stage = 0;
int remaining_time = 15;
int selected_option = 0;
bool redraw_needed = true;


void handle_game_state() {
    switch (state) {
        case MENU:
            draw_options(selected_option); 
            break;
            
        case PLAY:
            vg_draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000);
            
            //draw_hangman(current_stage);

            //To implement
            //draw_word_state();
            //draw_used_letters();
            
            gameCountdown(remaining_time);
            
            if (remaining_time == 0) {

                //when we implement the game, we will add (+1) to the current state of the hangman and if it is not in the "final form"
                //we dont change state, but if we are in the last form we end the game (state == EXIT e dar redraw)
                state = EXIT;
                redraw_needed = true;
            }
            break;

        case INSTRUCTIONS:
            break;
            
        case EXIT:
            draw_game_over_screen();

            //return to the menu after some time
            if (timer_counter % 60 == 0) {
                state = MENU;
                reset_game_state();
            }
            break;
    }
    redraw_needed = false;
}


void reset_game_state() {

    //clear the screen
    vg_draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000);

    state = MENU;
    remaining_time = 15; //we can create a new var
    current_stage = 0;
    selected_option = 0;
}
