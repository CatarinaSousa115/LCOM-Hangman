#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "k_controller.h"
#include "timer.c"
#include "i8254.h"

extern uint8_t scancode;
//extern uint8_t sys_counter; ativar isto?
extern uint32_t timer_counter;

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
  //if (kbd_print_no_sysinb(sys_counter) != 0) return 1; ativar isto?

  return 0;
}


//neste caso as interrupção serão desativadas quando se chamar o teste para conseguirmos usar tecnicas de polling
int(kbd_test_poll)() {

  //mesma logica para acabar o loop que o test_scan()
  while (scancode != ESC_BREAKCODE) {
    
    //neste caso como vamos estar sempre a verificar se foi escrito alguma coisa (polling), então só devemos dar print do scancode quando realmente
    //estivermos a ler alguma coisa de "OUT_BUFF"(0X60), quando "read_controller()" da return de 0
    if(read_controller(OUT_BUF, &scancode) == 0) {

      //print do scancode lido
      kbd_print_scancode(is_makecode(scancode), code_size(scancode), &scancode);
    }

    //caso contrário o "read_controller()" da return de 1, ou seja, ou existe um erro ou não existe nada para ler
  }

  if (restore_interruptions() != 0) {
        printf("Falha ao restaurar interrupções!\n");
        return 1;
  }

  return 0;
}


//neste caso, em vez de apenas tratarmos 1 tipo de interrupções iremos tratar 2, as do keyboard e do timer
int(kbd_test_timed_scan)(uint8_t n) {
  
  int ipc_status; //guarda o status da mensagem recebida 
  int r; //guarda o valor (0 ou 1) que permite saber se a mensagem foi bem recebida
  uint8_t irq_set_keyboard; //id da interrupção do teclado
  uint8_t irq_set_timer; //id da interrupção do timer
  message msg; //msg recebida pela interrupção

  uint8_t max_time_without_int = n;

  //"subscreve" a interrupção (pede para ser notificado quando a interrupção (que está em irq_set (???)) acontecer)
  if (keyboard_subscribe_int(&irq_set_keyboard) != 0) return 1; //interupção do teclado
  if (timer_subscribe_int(&irq_set_timer) != 0) return 1; //interupção do timer


  //neste caso o programa acaba se o scancode for "ESC" ou então se o timer chegar a 0, isto é, se nenhuma interrupção for feita nesse intervalo "n"
  while(scancode != ESC_BREAKCODE && n > 0) {
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) { /* received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */

            //não se pode usar else if porque podemos perder informações, ou do timer ou do teclado o que leva a falhar nos testes
            
            //caso o irq_set seja do timer (interrupção) vamos diminuir o contador de tempo max que pode passar sem uma interrupção do teclado ("n")
            if (msg.m_notify.interrupts & irq_set_timer) { 
              timer_int_handler();

              //basicamente verifica se já se passou 1seg (se o timer_counter atingiu um multiplo de 60 significa que passou mais 1 seg)
              if(timer_counter % 60 == 0){

                //ao ter passado mais 1 seg, o tempo max que pode existir sem interrupção do teclado diminui
                n--;
              }
            }

            //caso o irq_set seja do teclado vamos tratar da interrupção extamente como em "kbd_test_scan()"
            if (msg.m_notify.interrupts & irq_set_keyboard) {
             /*chamar interruption handler*/
             keyboard_ih();//tratar da interrupção
             kbd_print_scancode(is_makecode(scancode), code_size(scancode), &scancode); //da print do scancode
             
             //damos reset ao valor do "n" para o numero max original de tempo que pode passar sem inter. e aos "timer_counter" para começar a contar do 0
             n = max_time_without_int; 
             timer_counter = 0;
            }
        } 
      }
  }

  if (keyboard_unsubscribe_int() != 0) return 1;
  if (timer_unsubscribe_int() != 0) return 1;

  return 0;
}

