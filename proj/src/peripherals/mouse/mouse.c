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
int mouse_x = 150;
int mouse_y = 150;

extern uint8_t bytes_per_pixel;
extern uint8_t *back_buffer;

uint32_t *mouse_background = NULL;
int mouse_width = 32, mouse_height = 32;

void init_mouse_background(size_t bytes_per_pixel) {
  mouse_background = malloc(mouse_width * mouse_height * bytes_per_pixel);
  if (mouse_background == NULL) {
    printf("Failed to allocate memory for mouse background\n");
  }
}

extern int selected_option;

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

  printf("Status register: 0x%x\n", status);

  if ((status & OUT_BUF_FULL) && (status & AUX)) {
    if (read_controller(OUT_BUF, &curr_mouse_byte, 1) != 0) {
      printf("Error reading current mouse byte\n");
      return;
    }
    printf("Mouse byte received: 0x%x\n", curr_mouse_byte);
  }
  else {
    printf("Interrupt triggered without valid mouse data\n");
  }
}

void(buffer_mouse_bytes)() {
  // Ensure the first byte is valid and starts a new packet
  if ((curr_mouse_byte & BIT(3)) && packet_byte_index == 0) {
    mouse_bytes[packet_byte_index++] = curr_mouse_byte;
  }
  else if (packet_byte_index > 0 && packet_byte_index < 3) {
    mouse_bytes[packet_byte_index++] = curr_mouse_byte;
  }

  // Process the packet once all three bytes are received
  if (packet_byte_index == 3) {
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
    mouse_packet.delta_y = delta_y;

    // Update mouse position and handle clicks
    update_mouse_position_with_double_buffering();
    process_mouse_clicks();

    // Reset packet index for the next packet
    packet_byte_index = 0;
  }
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
  return 0;
}

void draw_mouse_pointer() {
  // Draw the mouse pointer at the current position
  for (int row = 0; row < mouse_img.height; row++) {
    for (int col = 0; col < mouse_img.width; col++) {
      uint32_t color = mouse_img.bytes[row * mouse_img.width + col];
      if (color != 0x0000) { // Skip transparent pixels
        vg_draw_pixel(mouse_x + col, mouse_y + row, color);
      }
    }
  }
}

void save_background(uint16_t mouse_x, uint16_t mouse_y) {
  for (int y = 0; y < mouse_height; y++) {
    for (int x = 0; x < mouse_width; x++) {
      int screen_x = mouse_x + x;
      int screen_y = mouse_y + y;

      if (screen_x >= h_res || screen_y >= v_res)
        continue;

      int index = (screen_y * h_res + screen_x) * bytes_per_pixel;
      int bg_index = (y * mouse_width + x) * bytes_per_pixel;

      memcpy(&mouse_background[bg_index], &back_buffer[index], bytes_per_pixel);
    }
  }
}

void restore_background(uint16_t mouse_x, uint16_t mouse_y) {
  for (int y = 0; y < mouse_height; y++) {
    for (int x = 0; x < mouse_width; x++) {
      int screen_x = mouse_x + x;
      int screen_y = mouse_y + y;

      if (screen_x >= h_res || screen_y >= v_res)
        continue;

      int index = (screen_y * h_res + screen_x) * bytes_per_pixel;
      int bg_index = (y * mouse_width + x) * bytes_per_pixel;

      memcpy(&back_buffer[index], &mouse_background[bg_index], bytes_per_pixel);
    }
  }
}

void update_mouse_position_logic() {
  // Restore the background at the previous mouse position
  restore_background(mouse_x, mouse_y);

  // Update horizontal position
  if (mouse_x + mouse_packet.delta_x < 0 && !mouse_packet.x_ov) {
    mouse_x = 0;
  }
  else if (mouse_x + mouse_packet.delta_x > SCREEN_WIDTH - mouse_img.width && !mouse_packet.x_ov) {
    mouse_x = SCREEN_WIDTH - mouse_img.width;
  }
  else if (!mouse_packet.x_ov) {
    mouse_x += mouse_packet.delta_x;
  }

  // Update vertical position
  if (mouse_y - mouse_packet.delta_y < 0 && !mouse_packet.y_ov) {
    mouse_y = 0;
  }
  else if (mouse_y - mouse_packet.delta_y > SCREEN_HEIGHT - mouse_img.height && !mouse_packet.y_ov) {
    mouse_y = SCREEN_HEIGHT - mouse_img.height;
  }
  else if (!mouse_packet.y_ov) {
    mouse_y -= mouse_packet.delta_y; // Invert delta_y for correct vertical movement
  }

  // Save the background at the new mouse position
  save_background(mouse_x, mouse_y);

  draw_mouse_pointer();
}

void process_mouse_clicks() {
  if (mouse_packet.lb) {
    printf("Left button clicked at (%d, %d)\n", mouse_x, mouse_y);
    handle_menu_click(mouse_x, mouse_y, &selected_option);
  }
}

void draw_mouse_pointer_to_back_buffer() {
  // Draw the mouse pointer to the back buffer
  for (int row = 0; row < mouse_img.height; row++) {
    for (int col = 0; col < mouse_img.width; col++) {
      uint32_t color = mouse_img.bytes[row * mouse_img.width + col];
      if (color != 0x0000) {                                // Skip transparent pixels
        vg_draw_pixel(mouse_x + col, mouse_y + row, color); // Use back buffer
      }
    }
  }
}

void update_mouse_position_with_double_buffering() {
  clear_back_buffer();

  update_mouse_position_logic();

  draw_mouse_pointer_to_back_buffer();

  swap_buffers();
}
