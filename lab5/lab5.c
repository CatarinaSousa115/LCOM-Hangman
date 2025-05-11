// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "i8042.h"
#include "k_controller.h"
#include "keyboard.h"

extern uint8_t scancode;
extern vbe_mode_info_t video_info;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (vg_init_graphic(mode) != 0) {
    return 1;
  }

  sleep(delay);

  if(vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
  //mapear a memoria que vamos usar
  if (map_video_memory(mode) != 0) {
    vg_exit();  
    return 1;
  }

  if (vg_init_graphic(mode) != 0) return 1;

  uint32_t new_color;
  if (normalize_color(color, &new_color) != 0) { //necessario para normalizar as corer por ex quando se usa o modo 110
    vg_exit();
    return 1;
  }

  //desenhar o retangulo
  if (vg_draw_rectangle(x, y, width, height, new_color) != 0) {
    vg_exit();
    return 1;
  }

  if (end_loop_ESC() != 0) {
    vg_exit();
    return 1;
  }

  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  
  if (map_video_memory(mode) != 0) {
    vg_exit();  
    return 1;
  }

  if (vg_init_graphic(mode) != 0) return 1;

  //como todos os retangulos vao ter o mesmo tamanho, primeiro calcular esse tamanho
  RectangleSize rectangle;
  if (calculate_size(&rectangle, no_rectangles, video_info.XResolution, video_info.YResolution) != 0) {
      vg_exit();
      return 1;
  }

  //com esse tamanho mandar desenhar o padrao
  if (draw_pattern(&rectangle, no_rectangles, first, step) != 0) {
      vg_exit();
      return 1;
  }

  if (end_loop_ESC() != 0) {
    vg_exit();
    return 1;
  }

  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  if (map_video_memory(MODE_105) != 0) {
    vg_exit();  
    return 1;
  }

  if (vg_init_graphic(MODE_105) != 0) return 1;

  xpm_image_t img;
  uint8_t *pixmap = xpm_load(xpm, XPM_INDEXED, &img);
  
  if (pixmap == NULL) {
    vg_exit();
    return 1;
  }

  if (draw_xpm(pixmap, &img, x, y) != 0) {
    vg_exit();
    return 1;
  }

  if (end_loop_ESC() != 0) {
    vg_exit();
    return 1;
  }

  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  
  int msg_status;
  message msg;
  uint8_t timer_irq, kbd_irq;

  //ativar as interrupções e configurar a frequencia desejada
  if (keyboard_subscribe_int(&kbd_irq) != 0) return 1;
  if (timer_subscribe_int(&timer_irq) != 0) return 1;
  if (timer_set_frequency(0, fr_rate) != 0) return 1;
  
  if (map_video_memory(MODE_105) != 0) {
    vg_exit();  
    return 1;
  }

  if (vg_init_graphic(MODE_105) != 0) return 1;

  //auxiliares
  int current_x = xi;
  int current_y = yi;

  int moving_horizontally = ((yi == yf) && (xi != xf));
  int moving_vertically = ((xi == xf) && (yi != yf));
  
  //movimento inválido
  if (!moving_horizontally && !moving_vertically) {
    vg_exit();
    return 1;
  }

  //preparar e desenhar o primeiro xpm
  xpm_image_t img;
  uint8_t *pixmap = xpm_load(xpm, XPM_INDEXED, &img);
  
  if (pixmap == NULL) {
    vg_exit();
    return 1;
  }

  if (draw_xpm(pixmap, &img, xi, yi) != 0) {
    vg_exit();
    return 1;
  }


  while (scancode != ESC_BREAKCODE && (xi < xf || yi < yf)) {
    if (driver_receive(ANY, &msg, &msg_status) != 0) continue;

    if (is_ipc_notify(msg_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & kbd_irq) {
            keyboard_ih();
          }

          if (msg.m_notify.interrupts & timer_irq) {
            
            if (vg_draw_rectangle(current_x, current_y, img.width, img.height, 0x000000) != 0) {
                vg_exit();
                return 1;
              }

              int move_amount;

              if (moving_horizontally) {
                move_amount = calculate_move_amount(speed, xf, current_x);
                current_x += move_amount;
              } 
              
              else { // moving_vertically
                move_amount = calculate_move_amount(speed, yf, current_y);
                current_y += move_amount;
              }

              // Redesenha na nova posição
              if (draw_xpm(pixmap, &img, current_x, current_y) != 0) {
                vg_exit();
                return 1;
              }
          }
          break;
      }
    }
  }

  if (vg_exit() != 0) return 1;

  if (timer_unsubscribe_int() != 0) return 1;
  if (keyboard_unsubscribe_int() != 0) return 1;

  return 0;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
