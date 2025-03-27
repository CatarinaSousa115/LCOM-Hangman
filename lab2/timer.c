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

  int port;

  if (timer == 0) {
    port = 0x40; //colocar o valor hexadecimal da porta para int
    command |= BIT(1);
  }

  if (timer == 1) {
    port = 0x41;
    command |= BIT(2);
  }

  if (timer == 2) {
    port = 0x42;
    command |= BIT(3);
  }

  sys_outb(TIMER_CTRL, command); //vou mandar para a porta 43 (porta de control word -- TIMER_CTRL) o comando que construi;

  util_sys_inb(port, st); //vou receber a resposta do "timer" e guardar a consiguração em *st;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  //fazer o display conf que nada mais é que: de acordo com o "field" vou preencher cada um dos seus pontos
  
  union timer_status_field_val helper;

  switch (field) {
  
  case tsf_all:
    helper.byte = st;
    break;

  case tsf_initial:
    st = (st >> 4); //shift 4 casas para a direita para os bits 4 e 5 (initl. mode) fiquem o mais a direita
    st = st & 0x3; //esta "mascara" deixa apenas ativos os bits 4 e 5 (pos antes de shift) desativando os outros

    if (st == 1) {
      helper.in_mode = LSB_only;
    }

    else if(st == 2) {
      helper.in_mode = MSB_only;
    } 

    else if(st == 3) {
      helper.in_mode = MSB_after_LSB;
    }

    else {
      helper.in_mode = INVAL_val;
    }
    break;

  case tsf_mode:
    st = (st >> 1); //shift 4 casas para a direita para os bits 1, 2 e 3 (oper. mode) fiquem o mais a direita
    st = st & 0x7; //esta "mascara" deixa apenas ativos os bits 1, 2 e 3 (pos antes de shift) desativando os outros 

    //basicamente agora iremos apenas fazer "if´s" para os casos em que mais de 1 val em st pode originar o mesmo op. mode 
    //neste caso temos que quando st == 6 ele ativa o modo 2 e quando é 7 o modo 3
    if (st == 6) {
      helper.count_mode = 2;
    }

    else if (st == 7) {
      helper.count_mode = 3;
    }

    //quando não é nenhum dos casos expecificos o op.mode vai corresponder ao valor do st
    else {
      helper.count_mode = st;
    }
    break;

  case tsf_base:
    st = st & TIMER_BCD; //o valor de TIMER_BCD é 0x1 e basicamente o que isto faz é: apenas fica ativo o bit 0 para conseguimos verificar se 
    //estamos no modo BCD ou não

    helper.bcd = st; //se o valor de st for 1 é true (bcd) se for 0 é false
    break;

  default:
    return 1;
  }

  if (!timer_print_config(timer, field, helper)) {
    return 0;
  }

  return 1;
}
