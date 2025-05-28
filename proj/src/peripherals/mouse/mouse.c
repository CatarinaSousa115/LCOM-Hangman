#include "mouse.h"
#include "../../assets/mouse_pixmap.h" // Include mouse pointer XPM
#include "../graphics/graphics.h"      // Include graphics functions

int mouse_hook_id = 3;
struct packet mouse_packet;
uint8_t packet_byte_index = 0;
uint8_t mouse_bytes[3];
uint8_t curr_mouse_byte;
static xpm_image_t mouse_img;
uint8_t *mouse_pixmap;
extern char *mouse_pointer_xpm[];
extern uint16_t h_res;
extern uint16_t v_res;
static int mouse_x = 150, mouse_y = 150; // Initial mouse position

int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }

  *bit_no = BIT(mouse_hook_id); // Save the bitmask for the interrupt

  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
  return (sys_irqrmpolicy(&mouse_hook_id));
}

void(mouse_ih)() {
  uint8_t status;

  if (read_controller(STAT_REG, &status, 1) != 0) {
    printf("Failed to read status register\n");
    return;
  }

  if ((status & OUT_BUF_FULL) && (status & AUX)) {
    if (read_controller(OUT_BUF, &curr_mouse_byte, 1) != 0) {
      printf("Error reading current mouse byte\n");
      return;
    }
  }
}

void(buffer_mouse_bytes)() {
  if ((curr_mouse_byte & BIT(3)) && packet_byte_index == 0) {
    mouse_bytes[packet_byte_index] = curr_mouse_byte;
    packet_byte_index++;
  }
  else if (packet_byte_index > 0) {
    mouse_bytes[packet_byte_index] = curr_mouse_byte;
    packet_byte_index++;
  }
}

void process_mouse_bytes() {
  // Existing logic to process mouse bytes
  for (int i = 0; i < 3; i++) {
    mouse_packet.bytes[i] = mouse_bytes[i];
  }
  mouse_packet.lb = (mouse_bytes[0] & MOUSE_LB);
  mouse_packet.rb = (mouse_bytes[0] & MOUSE_RB);
  mouse_packet.mb = (mouse_bytes[0] & MOUSE_MB);
  mouse_packet.x_ov = (mouse_bytes[0] & MOUSE_X_OVF);
  mouse_packet.y_ov = (mouse_bytes[0] & MOUSE_Y_OVF);

  int16_t delta_x = mouse_bytes[1];
  if (mouse_bytes[0] & MOUSE_X_SIGN) {
    delta_x |= 0xFF00; // Sign extend if the sign bit is set
  }
  mouse_packet.delta_x = delta_x;

  int16_t delta_y = mouse_bytes[2];
  if (mouse_bytes[0] & MOUSE_Y_SIGN) {
    delta_y |= 0xFF00; // Sign extend if the sign bit is set
  }
  mouse_packet.delta_y = -delta_y;
}

int(mouse_write_cmd)(uint8_t cmd) {
  uint8_t tries = MAX_MOUSE_TRIES;
  uint8_t response;

  while (tries--) {
    if (write_controller(KBC_WRITING, WRITE_BYTE_TO_MOUSE) != 0) {
      return 1;
    }
    if (write_controller(WRITE_CMD, cmd) != 0) {
      return 1;
    }
    if (util_sys_inb(OUT_BUF, &response) != 0) {
      return 1;
    }
    if (response == ACK) {
      return 0; // Command acknowledged
    }
    else if (response == ERROR)
      return 1;

    tickdelay(micros_to_ticks(20000)); // Delay before retrying
  }
  return 1; // Command not acknowledged after retries
}

int init_mouse_pointer() {
  // Load the mouse pointer XPM image
  mouse_pixmap = xpm_load((xpm_map_t) mouse_pointer_xpm, XPM_INDEXED, &mouse_img);
  if (mouse_pixmap == NULL) {
    printf("Failed to load mouse pointer XPM\n");
    return 1;
  }
  if (mouse_img.width > h_res || mouse_img.height > v_res) {
    printf("Pixmap exceeds screen bounds.\n");
    return 1;
  }
  if (draw_xpm(mouse_pixmap,&mouse_img,mouse_x, mouse_y) != 0) {
    printf("Failed to draw initial mouse pointer\n");
    return 1;
  }
  return 0;
}
