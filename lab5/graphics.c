#include <lcom/lcf.h>
#include "graphics.h"
#include "keyboard.h"


extern uint8_t scancode;

// Variáveis globais acessíveis pelas funções de desenho
vbe_mode_info_t video_info;
uint8_t *video_mem;
uint16_t h_res;
uint16_t v_res;
uint8_t bytes_per_pixel;

int end_loop_ESC() {
    int ipc_status;  
    int r; 
    uint8_t irq_set; 
    message msg; 

    if (keyboard_subscribe_int(&irq_set) != 0) return 1; 

    while(scancode != ESC_BREAKCODE) {      
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set) {
                        keyboard_ih(); 
                    }
                    break;
            }
        } 
    }

    if (keyboard_unsubscribe_int() != 0) return 1; 

    return 0; 
}

int vg_init_graphic(uint16_t mode) {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.ax = SET_VBE_MODE;
  reg86.bx = mode | BIT(14); //modo passado no registo bx com bit 14 ativo
  reg86.intno = 0x10; 

  if(sys_int86(&reg86) != 0) {
    return 1;
  }
  
  return 0;
}


int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= h_res || y >= v_res) return 1;  // fora do ecrã

  uint32_t index = (h_res * y + x) * bytes_per_pixel;

  memcpy(&video_mem[index], &color, bytes_per_pixel);
  
  return 0;
}


int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (unsigned i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) return 1;
  }

  return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  for (unsigned i = 0; i < height ; i++) {

    if (vg_draw_hline(x, y+i, width, color) != 0) return 1;
  }

  return 0;
}

int map_video_memory(uint16_t mode) {

  //vai obter informação sobre o modo grafico
  memset(&video_info, 0, sizeof(video_info));
  if(vbe_get_mode_info(mode, &video_info) != 0) {
    return 1;
  }

  //faz as contas de quantos bytes vai guardar cada pixel
  bytes_per_pixel = (video_info.BitsPerPixel + 7) / 8;
  h_res = video_info.XResolution;
  v_res = video_info.YResolution;

  //e depois o espaçe necessario para guardar tudo isso
  uint32_t vram_size = h_res * v_res * bytes_per_pixel;;

  struct minix_mem_range memory_range;
  memory_range.mr_base = video_info.PhysBasePtr;
  memory_range.mr_limit = memory_range.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &memory_range) != 0) return 1;
    
  video_mem = vm_map_phys(SELF, (void *)memory_range.mr_base, vram_size); //mapear a virtual memory)
  
  if (video_mem == NULL) return 1;

  return 0;
}


int normalize_color(uint32_t color, uint32_t *new_color) {
  
  //cor normal
  if (video_info.BitsPerPixel == 32) {
    *new_color = color;
  } 
  
  //temos de aplicar uma mascara para garantir que a cor não exceda o numero de bits suportado
  else {

    uint32_t mask = BIT(video_info.BitsPerPixel); //ativa um bit na possição n (usada pelo modo escolhido)
    mask -= 1; //subtraimos 1 para ativar apenas os n bits que vamos usar (n bits menos significativos)

    *new_color = color & mask;
  }

  return 0;
}

