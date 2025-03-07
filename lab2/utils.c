#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL)
    return 1;        // se o apontador for inválido retorna 1
  *lsb = val & 0xFF; // obter apenas os bits menos significativos
  return 0;          // retorna 0 se a função correr corretamente
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL)
    return 1;               // se o apontador for inválido retorna 1
  *msb = (val >> 8) & 0xFF; // obter apenas os bits mais significativos
  return 0;                 // retorna 0 se a função correr corretamente
}

int(util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL)
    return 1;                         // se o apontador for inválido retorna 1
  uint32_t aux_val;                   // valor auxiliar de 32 bites
  int ret = sys_inb(port, &aux_val);  // chama uma função auxiliar para ler o valor do port armazenado-o no valor auxiliar
  *value = aux_val & 0xFF             // obter apenas os bits menos significativos
  return ret;                         // retorna 0 se a função auxiliar tiver corrido corretamente se não, retorna outro valor
}
