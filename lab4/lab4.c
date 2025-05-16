// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdio.h>
#include <stdlib.h>

// Any header files included below this line should have been created by you
#include "mouse.h"

extern struct packet mouse_packet;
extern uint8_t packet_byte_index;
extern uint32_t timer_counter;
extern uint8_t mouse_bytes[3];
extern uint8_t curr_mouse_byte;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  message msg;
  uint8_t mouse_bit_no;
  if (mouse_subscribe_int(&mouse_bit_no) != 0)
    return 1;
  if (mouse_write_cmd(ENABLE_DR) != 0)
    return 1;

  while (cnt > 0) {
    // Get a request message.
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    // Check if the notification is for us and not for others
    if (is_ipc_notify(ipc_status)) {
      if (_ENDPOINT_P(msg.m_source) == HARDWARE) { // hardware interrupt notification
          if (msg.m_notify.interrupts & mouse_bit_no) { // check if it is our interrupt
            mouse_ih();
            buffer_mouse_bytes();
            if (packet_byte_index == 3) {
              process_mouse_bytes();
              mouse_print_packet(&mouse_packet);
              packet_byte_index = 0; // Reset for the next packet
              cnt--;
            }
          }
      }
    }
  }
  if (mouse_write_cmd(DISABLE_DR) != 0)
    return 1;
  if (mouse_unsubscribe_int() != 0)
    return 1;

  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  message msg;
  uint8_t mouse_bit_no;
  uint8_t timer_bit_no;
  uint16_t limit = sys_hz() * idle_time;

  if (mouse_subscribe_int(&mouse_bit_no) != 0)
    return 1;
  if (timer_subscribe_int(&timer_bit_no) != 0)
    return 1;

  if (mouse_write_cmd(ENABLE_DR) != 0)
    return 1;

  while (timer_counter < limit) {
    // Get a request message.
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    // Check if the notification is for us and not for others
    if (is_ipc_notify(ipc_status)) {
      if (_ENDPOINT_P(msg.m_source) == HARDWARE) { 
          if(msg.m_notify.interrupts & timer_bit_no){
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & mouse_bit_no) {
            mouse_ih();
            buffer_mouse_bytes();
            if (packet_byte_index == 3) {
              process_mouse_bytes();
              mouse_print_packet(&mouse_packet);
              packet_byte_index = 0; 
              timer_counter = 0; 
            }
          }
       
      }
    }
  }

  if (mouse_write_cmd(DISABLE_DR) != 0)
    return 1;

  if (mouse_unsubscribe_int() != 0)
    return 1;
  if (timer_unsubscribe_int() != 0)
    return 1;

  return 0;
}

typedef enum {
  INIT,
  DRAW_UP,
  VERTEX,
  DRAW_DOWN,
  COMPLETE,
  FAIL
} gesture_state_t;
int16_t x_movement = 0;
int16_t y_movement = 0;

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  message msg;
  uint8_t mouse_bit_no;
  gesture_state_t state = INIT;

  if (mouse_subscribe_int(&mouse_bit_no) != 0)
    return 1;

  if (mouse_write_cmd(ENABLE_DR) != 0)
    return 1;

  while (state != COMPLETE && state != FAIL) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & mouse_bit_no) {
            mouse_ih();
            buffer_mouse_bytes();
            if (packet_byte_index == 3) {
              process_mouse_bytes();
              mouse_print_packet(&mouse_packet);

              switch (state) {
                case INIT:
                  if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                    state = DRAW_UP;
                  }
                  break;
                case DRAW_UP:
                  if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                    if (mouse_packet.delta_x < -tolerance || mouse_packet.delta_y < -tolerance) {
                      state = FAIL;
                    }
                    else {
                      x_movement += mouse_packet.delta_x;
                      y_movement += mouse_packet.delta_y;
                    }
                  }
                  else if (!mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                    if (abs(x_movement) >= x_len && abs(y_movement) >= x_len && (abs(y_movement) > abs(x_movement)))
                      state = VERTEX;
                    else
                      state = FAIL;
                  }
                  break;

                case VERTEX:
                  if (!mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                    if (abs(mouse_packet.delta_x) > tolerance || abs(mouse_packet.delta_y) > tolerance)
                      state = FAIL;
                  }
                  else if (!mouse_packet.lb && mouse_packet.rb && !mouse_packet.mb) {
                    x_movement = 0;
                    y_movement = 0;
                    state = DRAW_DOWN;
                  }
                  else {
                    state = FAIL;
                  }
                  break;

                case DRAW_DOWN:
                  if (mouse_packet.rb && !mouse_packet.lb && !mouse_packet.mb) {
                    if (mouse_packet.delta_x < -tolerance || mouse_packet.delta_y > tolerance) {
                      state = FAIL;
                    }
                    else {
                      x_movement += mouse_packet.delta_x;
                      y_movement += mouse_packet.delta_y;
                    }
                  }
                  else if (!mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                    if (abs(x_movement) >= x_len && abs(y_movement) >= x_len && (abs(y_movement) > abs(x_movement)))
                      state = COMPLETE;
                    else
                      state = FAIL;
                  }
                  else {
                    state = FAIL;
                  }
                  break;

                case FAIL:
                  x_movement = 0;
                  y_movement = 0;
                  state = INIT;
                  break;

                default:
                  break;
              }
              packet_byte_index = 0;
            }
          }
          break;
      }
    }
  }

  if (mouse_write_cmd(DISABLE_DR) != 0)
    return 1;
  if (mouse_unsubscribe_int() != 0)
    return 1;

  return 0;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  uint8_t packet_counter = 0;
  while (packet_counter < cnt) {
    if (mouse_write_cmd(READ_DATA) != 0)
      return 1;

    mouse_ih();
    buffer_mouse_bytes();

    if (packet_byte_index == 3) {
      process_mouse_bytes();
      mouse_print_packet(&mouse_packet);
      packet_byte_index = 0;
      packet_counter++;

      tickdelay(micros_to_ticks(period * 1000));
    }
  }
  if (mouse_write_cmd(SET_STREAM_MODE) != 0)
    return 1;
  if (mouse_write_cmd(DISABLE_DR) != 0)
    return 1;

  uint8_t cmd_byte = minix_get_dflt_kbc_cmd_byte();

  if (write_controller(KBC_WRITING, WRITE_CMD) != 0)
    return 1;
  if (write_controller(WRITE_CMD, cmd_byte) != 0)
    return 1;

  return 0;
}
