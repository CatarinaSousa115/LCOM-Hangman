#include "menu.h"
#include "../assets/font.h"
#include "../peripherals/graphics/graphics.h"

void menu_init() {
  if (vg_init_graphic(0x14C) != 0) {
    printf("Failed to initialize graphics mode. Exiting...\n");
    return;
  }

  printf("Drawing test rectangle...\n");
  if (draw_letter('A', 150, 150, 0xFFFFFF) != 0) {
    printf("Failed to draw letter 'A'.\n");
  }
  return;
}


