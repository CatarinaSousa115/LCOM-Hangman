#include "menu.h"
#include "../assets/font.h"
#include "../peripherals/graphics/graphics.h"

void menu_init() {
  

  if (vg_draw_rectangle(150,150,100,100,0xFFFFFF) != 0) {
    printf("Failed to draw rectangle\n");
  }

   if (draw_string("Hello-world !", 180, 180, 0xF47685, 1) != 0) {
    printf("Failed to draw letter 'A'.\n");
  }

  
  return;
}



