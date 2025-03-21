#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ)
    return 1;
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  return 1;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void(timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  /*
  1. verificar o timer
  2. fazer o proposito da função
  3. verificar o resultado
  4. resolver
  */

  if (timer > 2 || timer < 0 || st == NULL)
    return 1; /*parar se o input for inválido*/
  uint8_t word_ctrl = 0x0;
  word_ctrl = BIT(7) | BIT(6) | BIT(5); /*ativar o read-back command, seguindo a tabela do i8254 (tbm se podia usar as macros destes bits)*/

  word_ctrl |= TIMER_RB_SEL(timer); /*adicionar o bit do timer recebido  à configuração*/

  if (sys_outb(0x43, word_ctrl) != 0)
    return 1; /*avisar i8254 com porta 0x43 (definida na macro TIMER_CTRL) com a palavra formada de que vamos ler a configuração*/
  if (util_sys_inb(TIMER_0 + timer, st))
    return 1; /* ler a configuração do registo associado ao timer, a macro 'TIMER_0' e o '+ timer' assegura que passamos a porta correta à função*/

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
