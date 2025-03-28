#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

int keyboard_hook_id = 1; //o hook id do keyboard é 1

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  //subscrever uma interrupção nada mais é do que pedir ao sistema para ser notificado sempre que um evento expecifico ocorrer

  //neste caso apenas fica ativo o bit da posição 1 (com a ajuda da "macro" BIT) de modo a que "bit_no" armazene a máscara de bits 
  //que representa a interrupção que irá ser subscrita
  *bit_no = BIT(keyboard_hook_id);

  //esta função vai dizer qual a "policy" usada para tratar a interupção expecifica e, enquanto ativa essa interrupção será sempre tratada
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , keyboard_hook_id) != 0) return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {
    if (!sys_irqrmpolicy(keyboard_hook_id)) return 1;
    
    return 0;
}

//vai lidar se é necessário tratar de algum erro do status do keyboard
int (keyboard_int_handler) () {

    uint8_t keyboard_status; //vai armazenar os status do keyboard

    //Chama-se esta função para ela ir até Status Register e receber o status do keyboard que vai ser analisado (para deteção de erros)
    util_sys_inb(STAT_REG, &keyboard_status);

    if ((keyboard_status & PARITY_ERROR) != 0)  { //se for diferente de 0, o bit do erro está ativo

        return 1;
    }

    if ((keyboard_status & TIMEOUT_ERROR) != 0) {

        return 1;
    }

    if ((keyboard_status & OUT_BUF_FULL) != 0) {

        return 1;
    }

    if ((keyboard_status & IN_BUFF_FULL) != 0) {

        return 1;
    }

}