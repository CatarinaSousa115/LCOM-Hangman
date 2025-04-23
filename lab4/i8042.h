#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

// KBC tem 2 registros na porta 0x64 e 0x60
#define STAT_REG     0x64
#define KBC_WRITING  0x64
#define OUT_BUF      0x60
#define WRITE_CMD    0x60
#define READ_CMD     0X20

//erros
#define PARITY_ERROR   BIT(7)
#define TIMEOUT_ERROR  BIT(6)
#define OUT_BUF_FULL   BIT(0)
#define IN_BUFF_FULL   BIT(1)

//scancode
#define ESC_BREAKCODE 0x81
#define MSB_SCANCODE  0xE0  // Byte mais significativo do scancode de dois bytes

//mouse
#define MOUSE_LB            BIT(0)
#define MOUSE_RB            BIT(1)
#define MOUSE_MB            BIT(2)
#define IS_CONTROL_BYTE     BIT(3)
#define MSB_X_DELTA         BIT(4)
#define MSB_Y_DELTA         BIT(5)
#define MOUSE_X_OVFL        BIT(6)
#define MOUSE_Y_OVFL        BIT(7)
#define DATA_REPORT_OFF     0xF5
#define DATA_REPORT_ON      0xF4
#define STREAM_MODE_ON      0xEA
#define MOUSE_READ_DATA     0xEB
#define MOUSE_BYTE_WRITE    0xD4

#define ACK                 0xFA
#define NACK                0xFE
#define ERROR               0xFC

#endif

