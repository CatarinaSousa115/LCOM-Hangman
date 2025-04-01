#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>

int keyboard_hook_id = 1; //o hook id do keyboard é 1
uint8_t scancode = 0;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  //subscrever uma interrupção nada mais é do que pedir ao sistema para ser notificado sempre que um evento expecifico ocorrer

  //neste caso apenas fica ativo o bit da posição 1 (com a ajuda da "macro" BIT) de modo a que "bit_no" armazene a máscara de bits 
  //que representa a interrupção que irá ser subscrita
  *bit_no = BIT(keyboard_hook_id);

  //esta função vai dizer qual a "policy" usada para tratar a interupção expecifica e, enquanto ativa essa interrupção será sempre tratada
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &keyboard_hook_id) != 0) return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {
    if (!sys_irqrmpolicy(&keyboard_hook_id)) return 1;
    
    return 0;
}

//vai verificar se existem/é necessário tratar de algum erro do status do keyboard
int (validate_keyboard_status) () {

    uint8_t attemps = 10;
    uint8_t keyboard_status; //vai armazenar os status do keyboard

    while (attemps > 0) {

        //Chama-se esta função para ela ir até Status Register e receber o status do keyboard que vai ser analisado (para deteção de erros)
        if (util_sys_inb(STAT_REG, &keyboard_status) != 0) return 1;

        if ((keyboard_status & OUT_BUF_FULL) != 0) { //O Output buffer está full, logo existem dados para ler/ser processados
            
            if ((keyboard_status & PARITY_ERROR) != 0)  { //se for diferente de 0, o bit do erro está ativo
                //erro no parity
                return 1;
            }

            if ((keyboard_status & TIMEOUT_ERROR) != 0) {
                //erro timeout
                return 1;
            }

            //se não existir erro no status (verificado pela leitura da porta 0x64 (status register)) então podemos passar a leitura do scancode
            //que irá estar na porta 0x60 (output buffer) e processa-lo posteriormente
            if (util_sys_inb(OUT_BUF, &scancode) != 0) {
                return 1;
            }

            return 0;
        }

        tickdelay(micros_to_ticks(30000));
        attemps--;
    }

    return 1;
}

void (keyboard_ih) () {
    if (validate_keyboard_status() != 0) {
        printf("Error reading the scancode");
    }
}

//retorna true se for makecode, falso se for breakcode
bool (is_makecode)(uint8_t scancode){
    if(scancode & BIT(7)){  //the scancode when a key is released (break code) is the make code of that key with the MSB set to 1
        return false;
    }
    return true;
}

int (code_size)(uint8_t scancode){
    if(scancode == MSB_SCANCODE){
        return 2;
    }

    return 1;
}

