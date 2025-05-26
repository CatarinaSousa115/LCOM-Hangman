// Initializes peripherals, starts the game loop.

#include "assets/pixmap.h"
#include "assets/font.h"
#include "game/hangman.h"
#include "game/menu.h"
#include "peripherals/graphics/VBE.h"
#include "peripherals/graphics/graphics.h"
#include "peripherals/i8042.h"
#include "peripherals/i8254.h"
#include "peripherals/keyboard/keyboard.h"
#include "peripherals/mouse/mouse.h"

extern uint8_t packet_byte_index;
uint8_t irq_kb, irq_mouse, irq_timer;

bool gameRunning = true;

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

  if (map_video_memory(0x14C) != 0) {
    return 1;
  }

  // Initialize graphics mode
  if (vg_init_graphic(0x14C) != 0) {
    printf("Failed to initialize graphics mode. Exiting...\n");
    return 1;
  }

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
  // Unsubscribe from interrupts
  if (mouse_unsubscribe_int())
    return 1;
  if (keyboard_unsubscribe_int())
    return 1;
  if (timer_unsubscribe_int())
    return 1;

  return 0;
}

int process_interrupts() {
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
            // Add timer-related logic here
          }
          // Keyboard interrupt
          if (msg.m_notify.interrupts & irq_kb) {
            keyboard_ih();
            if (scancode == 0x01) {
              gameRunning = false; // Exit the game loop
            }
          }
          // Mouse interrupt
          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            buffer_mouse_bytes();
            if (packet_byte_index == 3) {
              process_mouse_bytes();
              packet_byte_index = 0; // Reset for the next packet
            }
          }
          break;
        default:
          break;
      }
    }
  }

  // Unsubscribe from interrupts
  if (mouse_unsubscribe_int())
    return 1;
  if (keyboard_unsubscribe_int())
    return 1;
  if (timer_unsubscribe_int())
    return 1;

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {

  if (init_devices())
    return 1;



  menu_init();
  // Exit graphics mode
   if (process_interrupts() != 0) {
    printf("Error during interrupt processing.\n");
    return 1;
  }

  remove_devices();
  graphics_exit();
  return 0;
}
