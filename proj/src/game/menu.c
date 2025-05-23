#include "menu.h"
#include "../peripherals/graphics/graphics.h"

void menu_init() {
    if (vg_init_graphic(0x14C) != 0) {
        printf("Failed to initialize graphics mode. Exiting...\n");
        return;
    }

    printf("Drawing test rectangle...\n");
    if (vg_draw_rectangle(100, 100, 200, 100, 0xF800) != 0) { // Red rectangle
        printf("Failed to draw test rectangle.\n");
        return;
    }
}

