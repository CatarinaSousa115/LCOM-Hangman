#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

// KBC tem 2 registros na porta 0x64 e 0x60
#define STAT_REG 0x64
#define KBC_WRITING 0x64
#define OUT_BUF 0x60
#define WRITE_CMD 0x60
#define READ_CMD 0X20

//erros
#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define OUT_BUF_FULL BIT(0)
#define IN_BUFF_FULL BIT(1)
#define AUX BIT(5) //bit 5 do status register, que indica se o dado lido é do teclado ou do mouse

//scancodes
#define ESC_BREAKCODE 0x81
#define MSB_SCANCODE 0xE0  // Byte mais significativo do scancode de dois bytes

#define KEY_UP_ARROW 0x48
#define KEY_DOWN_ARROW 0x50
#define KEY_ENTER 0x1C

#define SCANCODE_A 0x1E
#define SCANCODE_B 0x30
#define SCANCODE_C 0x2E
#define SCANCODE_D 0x20
#define SCANCODE_E 0x12
#define SCANCODE_F 0x21
#define SCANCODE_G 0x22
#define SCANCODE_H 0x23
#define SCANCODE_I 0x17
#define SCANCODE_J 0x24
#define SCANCODE_K 0x25
#define SCANCODE_L 0x26
#define SCANCODE_M 0x32
#define SCANCODE_N 0x31
#define SCANCODE_O 0x18
#define SCANCODE_P 0x19
#define SCANCODE_Q 0x10
#define SCANCODE_R 0x13
#define SCANCODE_S 0x1F
#define SCANCODE_T 0x14
#define SCANCODE_U 0x16
#define SCANCODE_V 0x2F
#define SCANCODE_W 0x11
#define SCANCODE_X 0x2D
#define SCANCODE_Y 0x15
#define SCANCODE_Z 0x2C

//mouse
#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define STATUS_BYTE BIT(3)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)
#define DISABLE_DR 0xF5
#define ENABLE_DR 0xF4
#define WRITE_BYTE_TO_MOUSE 0xD4
#define SET_STREAM_MODE 0xEA
#define READ_DATA 0xEB

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define MAX_MOUSE_TRIES 5


#endif




