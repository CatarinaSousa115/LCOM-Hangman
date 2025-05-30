// Initializes peripherals, starts the game loop.

#include "assets/font.h"
#include "assets/game_pixmap.h"
#include "peripherals/graphics/VBE.h"
#include "peripherals/graphics/graphics.h"
#include "peripherals/i8042.h"
#include "peripherals/i8254.h"
#include "peripherals/keyboard/keyboard.h"
#include "peripherals/mouse/mouse.h"
#include "game/hangman.h"
#include "game/game_state.h"
#include "game/menu.h"
#include "game/game.h"

#include <time.h>

extern uint32_t timer_counter;
extern uint8_t scancode;
extern uint8_t packet_byte_index;
extern int remaining_time;
extern int selected_option;
extern bool redraw_needed;
extern int mouse_x, mouse_y;


uint8_t irq_kb, irq_mouse, irq_timer;
bool gameRunning = true;
StateOptions state = MENU;


int(main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();
  return 0;
}

int init_devices() {

  if (map_main_bufferory(0x105) != 0) {
    return 1;
  }

  // Initialize graphics mode
  if (vg_init_graphic(0x105) != 0) {
    printf("Failed to initialize graphics mode. Exiting...\n");
    return 1;
  }

  if (init_double_buffer() != 0) {
    return 1;
  }

  timer_set_frequency(0, 60);

  // Subscribe to interrupts
  if (timer_subscribe_int(&irq_timer))
    return 1;
  if (keyboard_subscribe_int(&irq_kb))
    return 1;
  if (mouse_subscribe_int(&irq_mouse))
    return 1;

  // Enable mouse data reporting
  if (mouse_enable_data_reporting())
    return 1;

  return 0;
}

int remove_devices() {
  free_double_buffer();

  if (vg_exit() != 0)
    return 1;

  // Unsubscribe from interrupts
  if (mouse_unsubscribe_int()) {
    printf("Error during mouse processing.\n");
    return 1;
  }
  if (keyboard_unsubscribe_int()) {
    printf("Error during keyboard processing.\n");
    return 1;
  }
  if (timer_unsubscribe_int()) {
    printf("Error during timer processing.\n");
    return 1;
  }
  return 0;
}

int game_loop() {
  int ipc_status;
  message msg;

  while (gameRunning) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // Timer interrupt
          if (msg.m_notify.interrupts & irq_timer) {
            timer_int_handler();

            if (timer_counter % 60 == 0) {
              redraw_needed = true; 
            } 

            //if we are playing decrease the remaining time
            if ((timer_counter % 30 == 0) && (state == PLAY)) {
              redraw_needed = true; 
              remaining_time--;
            }
          }
          
          // Keyboard interrupt
          if (msg.m_notify.interrupts & irq_kb) {
            keyboard_ih();

            if (state == MENU) {
              // Pass the scancode to handle_menu_input
              handle_menu_input(scancode, &selected_option);
              redraw_needed = true;
            }

            if (state == PLAY) {
              if (scancode == ESC_BREAKCODE) {    
                reset_game_state();
              }

              if (handle_game_input(scancode) == 0) {
                redraw_needed = true;
              }
            }

            if (scancode == ESC_BREAKCODE && state == INSTRUCTIONS) {
              state = MENU;
              clear_screen();
              redraw_needed = true;
              }
          }

          // Mouse interrupt
          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            buffer_mouse_bytes();
            if (packet_byte_index == 3) {
              process_mouse_bytes();
              process_mouse_clicks();
              packet_byte_index = 0; // Reset for the next packet
            }

            if (state == MENU) {
              update_selected_option(mouse_x, mouse_y, &selected_option);
            }
          }
          break;
        default:
          break;
      }
    }

   if (redraw_needed) {
      clear_back_buffer();       
      handle_game_state();       
      swap_buffers();          
      redraw_needed = false;
  }
  }
  return 0;
}


int(proj_main_loop)(int argc, char *argv[]) {

  if (init_devices())
    return 1;

  if(init_mouse_pointer() != 0){
    return 1;
  }

  if (game_loop() != 0) {
    printf("Error during interrupt processing.\n");
    remove_devices();
    return 1;
  }

  remove_devices();
  return 0;
}
