#include "../peripherals/graphics/graphics.h"  
#include "../assets/font.h"                  
#include "../assets/game_pixmap.h"

#include "words.h"           
#include "hangman.h"          
#include "menu.h"             
#include "instructions.h"     
#include "../game/game.h"    
#include "game_state.h"


int current_stage = 0;
int remaining_time = 30;
int selected_option = 0;
bool redraw_needed = true;
bool is_setup = true;


void handle_game_state() {
    switch (state) {
        case MENU:
            draw_options(selected_option); 
            break;
   
        case PLAY:
            if(is_setup) {
               vg_draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000);
               generate_guessword();
               is_setup = false;
            }
    
            draw_word_guesses();
            draw_hangman(100,100, current_stage);
            //To implement
            //draw_used_letters();
            
            gameCountdown(remaining_time);
                                        //max hangman stage
            if (remaining_time == 0 || current_stage > 6) {

                //when we implement the game, we will add (+1) to the current state of the hangman and if it is not in the "final form"
                //we dont change state, but if we are in the last form we end the game (state == EXIT e dar redraw)
                state = EXIT;
                redraw_needed = true;
            }
            break;

        case INSTRUCTIONS:
                draw_instructions_screen();
            break;
            
        case EXIT:
            draw_game_over_screen();

            if(timer_counter % 60 == 0){
                reset_game_state();
                is_setup = true;
                redraw_needed = true;
            
            }
            break;

    }
    redraw_needed = false;
}


void reset_game_state() {

    //clear the screen
    vg_draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000);

    state = MENU;
    remaining_time = 30; //we can create a new var
    current_stage = 0;
    selected_option = 0;
    memset(guessed_letters, 0, sizeof(guessed_letters)); //clear the gessed_letters (all false again)
}
