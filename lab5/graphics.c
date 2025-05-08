#include <lcom/lcf.h>
#include "graphics.h"

vbe_mode_info_t vmi;
uint8_t *framebuffer;
uint16_t h_res;
uint16_t v_res;
uint8_t bits_per_pixel;
uint32_t bytes_per_scanline;


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

int init_framebuffer(uint16_t mode){
  // buscar informações do modo
  memset(&vmi, 0, sizeof(vmi));
  if(vbe_get_mode_info(mode, &vmi) != 0) {
    return 1;
  }

  // calcular número de bytes que o framebuffer ocupa
  unsigned int bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
  unsigned int size = vmi.XResolution * vmi.YResolution * bytes_per_pixel;

  // alocar mem. física e mapeá-la para o espaço virtual
  struct minix_mem_range mem_range;
  mem_range.mr_base = vmi.PhysBasePtr;
  mem_range.mr_limit = mem_range.mr_base + size;
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range) != OK) {
    return 1;
  }

  // guardar o pointer para o framebuffer
  framebuffer = vm_map_phys(SELF, (void *)mem_range.mr_base, size);
  if (framebuffer == NULL) {
    return 1;
  }


  return 0;
}

uint16_t convert_color_to_mode(uint32_t color) {
  // Extrair 8 bits de R, G, B da color
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  // Converter no formato 5:5:5 
  uint16_t r5 = (r >> 3) & 0x1F;
  uint16_t g5 = (g >> 3) & 0x1F;
  uint16_t b5 = (b >> 3) & 0x1F;

  return (r5 << 10) | (g5 << 5) | b5;
}


int this_vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
 if (x >= vmi.XResolution || y >= vmi.YResolution) {
    return 1; 
  }

  unsigned bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
  unsigned int index = (vmi.XResolution * y + x) * bytes_per_pixel;

  if (vmi.BitsPerPixel == 15) {
    uint16_t conv = convert_color_to_mode(color);
    memcpy(&framebuffer[index], &conv, 2);
  } else {
    memcpy(&framebuffer[index], &color, bytes_per_pixel);
  }
  


  return 0;
}

int this_vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (unsigned i = 0; i < len; i++) {
    if (this_vg_draw_pixel(x + i, y, color) != 0) {
      return 1; // erro ao desenhar o pixel
    }
  }

  return 0;
}

int this_vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  // Desenhar a linha horizontal superior
  for (unsigned i = 0; i < height; i++) {
    if (this_vg_draw_hline(x, y + i, width, color) != 0) {
      vg_exit();
      return 1; // erro ao desenhar a linha
    }
  }

  return 0;
}




