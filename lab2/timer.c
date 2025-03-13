#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(timer > 2 || timer < 0 || freq > TIMER_FREQ || freq < 19) return 1; /*validação de input*/
  uint8_t old_config, new_config;
  if(timer_get_conf(timer, old_config) != 0) return 1; /*guardar a configuração antiga*/

  new_config = (old_config & 0x0F) | BIT(5) | BIT(4); /*construção do comando de configuração*/
  switch(timer){
    case 0:
    break;
    case 1:
    new_config |= BIT(6);
    break;
    case 2:
    new_config |= BIT(7);
    break;
  }

  uint32_t counter_val = TIMER_FREQ / freq; /*valor inicial do contador*/
  uint8_t msb, lsb;
  util_get_LSB(counter_val, &lsb);
  util_get_MSB(counter_val, &msb);

  if(sys_outb(0x43, new_config) != 0) return 1; /*avisar i8254 que vamos configurar o timer*/

  if (sys_outb(TIMER_0 + timer, LSB) != 0) return 1; /*colocar o valor do counter através de lsb followed by msb*/
  if (sys_outb(TIMER_0 + timer, MSB) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  

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
  if(timer > 2 || timer < 0 || st == NULL) return 1; /*validação de input*/
  uint8_t ctrl_word = BIT(7) | BIT(6) | BIT(5); /*ativação do read-back command, conforme a tabela de consulta do i8254 (alternativamente, podia-se usar as macros destes bits)*/

  ctrl_word |= TIMER_RB_SEL(timer);  /*adicionar à configuração o bit do timer recebido*/
  
  if(sys_outb(0x43, ctrl_word) != 0) return 1; /*avisar i8254 com porta 0x43 (definida na macro TIMER_CTRL) com a palavra formada de que vamos ler a configuração*/
  if(util_sys_inb(TIMER_0 + timer, st)) return 1; /* ler a configuração do registo associado ao timer, a macro 'TIMER_0' e o '+ timer' assegura que passamos a porta correta à função*/
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
