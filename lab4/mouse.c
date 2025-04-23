#include "mouse.h"

int mouse_hook_id = 3; //hook id do mouse
uint8_t cur_packet_byte = 0; //o rato envia 3 bytes mas so vamos receber 1 por vez

struct packet mouse_packet;

uint8_t packet_bytes[3]; //3 bytes que o rato envia
uint8_t cur_index = 0; //index atual do packet_bytes
uint8_t cur_byte = 0; //byte atual

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = BIT(mouse_hook_id);

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &mouse_hook_id) != 0) return 1;

  return 0;
}


int (mouse_unsubscribe_int)() {
    if (!sys_irqrmpolicy(&mouse_hook_id)) return 1;
    
    return 0;
}

//o "mouse_ih" vai ser um dos bytes do packet do rato (= 1)
void (mouse_ih)() {
    if (read_controller(OUT_BUF, &cur_packet_byte, 1) != 0) {
        printf("Error reading the packet_byte");
    }
}

void (organize_packet_bytes)() {
    uint8_t is_control = (cur_byte & IS_CONTROL_BYTE); //se o byte 3 estiver ativo e byte de control

    //serve para reservar a posição 0 do packet_bytes para receber o control_byte
    if ((cur_index == 0) && is_control) {
        packet_bytes[cur_index] = cur_byte;
        cur_index++;
    }

    //nao e o byte de control logo recebe o delta_x e delta_y
    else if (cur_index > 0){
        packet_bytes[cur_index] = cur_byte;
        cur_index++;
    }
}

void (constuct_packet)() {

}
