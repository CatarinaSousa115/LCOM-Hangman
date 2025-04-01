#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ 1

// KBC tem 2 registros na porta 0x64 e 0x60
#define STAT_REG 0x64
#define KBC_WRITING 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define READ_CMD 0X20

//erros
#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define OUT_BUF_FULL BIT(0)
#define IN_BUFF_FULL BIT(1)

//scancode
#define ESC_BREAKCODE 0x81
#define MSB_SCANCODE 0xE0  // Byte mais significativo do scancode de dois bytes

#endif

