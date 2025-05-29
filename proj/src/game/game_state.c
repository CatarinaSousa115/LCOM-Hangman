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
int remaining_time = TIME_LIMIT
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
               clear_screen();
               generate_guessword();
               is_setup = false;
            }

            if (remaining_time == 0) {

                if(current_stage <= 6) {
                    current_stage++;
                    remaining_time = TIME_LIMIT
                }

                else {
                    //when we implement the game, we will add (+1) to the current state of the hangman and if it is not in the "final form"
                    //we dont change state, but if we are in the last form we end the game (state == EXIT e dar redraw)
                    state = EXIT;
                }

                redraw_needed = true;
            }

            if (current_stage > 6) {
                state = EXIT;
                redraw_needed = true;
            }
    
            draw_word_guesses();
            draw_hangman(5, 150, current_stage);
            //To implement
            //draw_used_letters();
            
            gameCountdown(remaining_time);
                                        //max hangman stage
            
            if(check_win()) {
                clear_screen();
                state = WIN;
                redraw_needed = true;
            }

            break;

        case WIN:      
            draw_string("You win!", 500, 400, TEXT_COLOR, 3);
                        
            if (timer_counter % 180 == 0) {
                reset_game_state();
                is_setup = true;
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
    clear_screen();

    state = MENU;
    remaining_time = TIME_LIMIT; //we can create a new var
    current_stage = 0;
    selected_option = 0;
    memset(guessed_letters, 0, sizeof(guessed_letters)); //clear the gessed_letters (all false again)
}
