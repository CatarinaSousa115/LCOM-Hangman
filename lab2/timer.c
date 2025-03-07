#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer != 0 && timer != 1 && timer != 2) { // o timer recebe 0,1 ou 2 de acordo com a porta (unsigned de 8 bits = int)
    return 1;
  }

  uint8_t command = BIT(7) | BIT(6) | BIT(5); //ativar o bit 7 e 6 para ativar o read-back, ativo o BIT(5) não ativo o bit 4 pois queremos receber 
                                            //apenas a configuração       <----- atenção que não sei se o bit 5 deve ser ativado

  switch (timer)
  {
  case 0:
    command |= BIT(1);
    break;
  
  case 1:
    command |= BIT(2);
    break;

  case 2:
    command |= BIT(3);
    break;
  }

  sys_outb(0x43, command); //vou mandar para a porta 43 (porta de control word) o comando que construi;

  until_sys_inb(timer, *st); //vou receber a resposta do "timer" e guardar a consiguração em *st;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  //fazer o display conf que nada mais é que: de acordo com o "field" vou preencher cada um dos seus pontos

  return 1;
}
