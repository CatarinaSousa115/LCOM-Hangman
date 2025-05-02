#include <lcom/lcf.h>
#include "graphics.h"


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


