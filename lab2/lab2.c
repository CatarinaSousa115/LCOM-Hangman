#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern uint32_t timer_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t helper;
  timer_get_conf(timer, &helper);
  timer_display_conf(timer, helper, field);

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(timer_test_int)(uint8_t time) {
  
  int ipc_status; //guarda o status da mensagem recebida 
  int r; //guarda o valor (0 ou 1) que permite saber se a mensagem foi bem recebida
  uint8_t irq_set; //id da interrupção
  message msg; //msg recebida pela interrupção

  //"subscreve" a interrupção (pede para ser notificado quando a interrupção (que está em irq_set (???)) acontecer)
  if (timer_subscribe_int(&irq_set) != 0) {
    return 1;
  }

  while(time > 0) { //enquanto tiver "tempo no relógio" continua
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) { /* received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */

            if (msg.m_notify.interrupts & irq_set) {
              timer_int_handler(); //conta o numero de interrupções 

              //basicamente verifica se já se passou 1seg (se o timer_counter atingiu um multiplo de 60 significa que passou mais 1 seg)
              if(timer_counter % 60 == 0){

                //se isso tiver acontecido da print ao tempo que já passou e reduz o tempo que ainda falta passar ("time")
                timer_print_elapsed_time(); //se não quiser ver o tempo a passar na linha de comandos posso simplestemnte ignorar esta linha
                time--;
              }
            }
            break;
          default:
            break; /* no other notifications expected: do nothing */ 
        }
      } else { /* received a standard message, not a notification */ 
          /* no standard messages expected: do nothing */
      }
  }

  //"para" com a subscrição
  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}
