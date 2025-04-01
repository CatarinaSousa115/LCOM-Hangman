#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"


extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status; //guarda o status da mensagem recebida 
  int r; //guarda o valor (0 ou 1) que permite saber se a mensagem foi bem recebida
  uint8_t irq_set; //id da interrupção
  message msg; //msg recebida pela interrupção

  //"subscreve" a interrupção (pede para ser notificado quando a interrupção (que está em irq_set (???)) acontecer)
  if (keyboard_subscribe_int(&irq_set) != 0) return 1;

  while(scancode != ESC_BREAKCODE) {//se o scancode for "ESC" é para parar o programa
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) { /* received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */

            if (msg.m_notify.interrupts & irq_set) {
             /*chamar interruption handler*/
             keyboard_ih();//tratar da interrupção

             kbd_print_scancode(is_makecode(scancode), code_size(scancode), &scancode);
              
            }
        }
      } 
  }

  if (keyboard_unsubscribe_int() != 0) return 1;

  return 0;
}


int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

