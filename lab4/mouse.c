#include "mouse.h"

int mouse_hook_id = 3; //hook id do mouse
//uint8_t cur_packet_byte = 0; //o rato envia 3 bytes mas so vamos receber 1 por vez

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
    if (read_controller(OUT_BUF, &cur_byte, 1) != 0) {
        printf("Error reading the packet_byte");
    }
}

//para garantir que os bytes são do mesmo packet
void (organize_packet_bytes)(uint8_t *packet_count) {
    uint8_t is_control = (cur_byte & IS_CONTROL_BYTE); //se o bit 3 estiver ativo e é byte de control

    //serve para reservar a posição 0 do packet_bytes para receber o control_byte
    if ((cur_index == 0) && is_control) {
        packet_bytes[cur_index] = cur_byte;
        cur_index++;
    }

    //packet já foi construído (bytes 0 a 2)
    else if (cur_index == 2) {
        packet_bytes[cur_index] = cur_byte;
        cur_index++;
        construct_packet(); // chamado logo após o 3º byte
    }
    

    //não é o byte de controlo logo recebe o delta_x e delta_y
    else if (cur_index > 0){
        packet_bytes[cur_index] = cur_byte;
        cur_index++;
    }
}

void (construct_packet)() {
    //colocar os bytes do array packet_bytes no struct do packet em si
    for (int i = 0 ; i < 3 ; i++) {
        mouse_packet.bytes[i] = packet_bytes[i];
    }

    //atribuir os valores corretos ao mouse_packet 
    mouse_packet.lb = packet_bytes[0] & MOUSE_LB;
    mouse_packet.mb = packet_bytes[0] & MOUSE_MB;
    mouse_packet.rb = packet_bytes[0] & MOUSE_RB;
    mouse_packet.x_ov = packet_bytes[0] & MOUSE_X_OVFL;
    mouse_packet.y_ov = packet_bytes[0] & MOUSE_Y_OVFL;

    //Se o deslicamento for negativo (MSB_X ou MSB_Y de delta ativo) então presevar só os 8 primeiros bits, caso contrario preservar os 16 bits
    mouse_packet.delta_x = (packet_bytes[0] & MSB_X_DELTA) ? (0xFF00 | packet_bytes[1]) : packet_bytes[1];
    mouse_packet.delta_y = (packet_bytes[0] & MSB_Y_DELTA) ? (0xFF00 | packet_bytes[2]) : packet_bytes[2];
}

int (mouse_config)(uint8_t config) {

uint8_t attemps = 10;
uint8_t mouse_response;

do {
    attemps--;
    if (write_controller(STAT_REG, MOUSE_BYTE_WRITE)) return 1;
    if (write_controller(OUT_BUF, config)) return 1;
    tickdelay(micros_to_ticks(3000));
    if (util_sys_inb(OUT_BUF, &mouse_response)) return 1;
    if (mouse_response == ACK) return 0;
  } while (mouse_response != ACK && attemps);

  return 1;
}
