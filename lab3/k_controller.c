#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "k_controller.h"
#include <stdbool.h>
#include <stdint.h>

// Este ficheiro contém funções para comunicação com o controlador de teclado (KBC)
// Inclui operações para ler do output buffer e escrever no input buffer do KBC,
// com verificação de erros de paridade e timeout.

// Lê um byte do controlador de teclado (KBC) a partir da porta especificada
// Garante que há dados disponíveis e que não há erros de paridade ou timeout
// Retorna 0 em caso de sucesso, 1 em caso de erro
int (read_controller) (uint8_t port, uint8_t *data) {

    // Tenta no máximo 10 vezes para evitar espera infinita caso o KBC não esteja pronto
    uint8_t attemps = 10;
    uint8_t keyboard_status; //vai armazenar os status do keyboard

    while (attemps > 0) {

        //Chama-se esta função para ela ir até Status Register e receber o status do keyboard que vai ser analisado (para deteção de erros)
        if (util_sys_inb(STAT_REG, &keyboard_status) != 0) return 1;

        if ((keyboard_status & OUT_BUF_FULL) != 0) { //O Output buffer está full, logo existem dados para ler/ser processados
            
            if ((keyboard_status & PARITY_ERROR) != 0) return 1;//se for diferente de 0, o bit do erro está ativo (erro no parity)

            if ((keyboard_status & TIMEOUT_ERROR) != 0) return 1; //erro timeout

            //se não existir erro no status (verificado pela leitura da porta 0x64 (status register)) então podemos passar a leitura da "data"
            //que irá estar na "port" e processa-la posteriormente
            if (util_sys_inb(port, data) != 0) return 1;

            return 0;
        }

        // Aguarda 30 ms para dar tempo ao KBC de processar os dados (evita busy-waiting agressivo)
        tickdelay(micros_to_ticks(30000)); //deelay devido a lentidão do KBC para processar os dados
        attemps--;
    }

    return 1;
}


// Escreve um byte no controlador de teclado (KBC) através da porta especificada
// Garante que o input buffer está vazio antes de escrever
// Retorna 0 em caso de sucesso, 1 em caso de erro
int (write_controller) (uint8_t port, uint8_t data) {

    // Tenta no máximo 10 vezes para evitar espera infinita caso o KBC não esteja pronto para receber dados
    uint8_t attemps = 10;
    uint8_t keyboard_status; //vai armazenar os status do keyboard

    while (attemps > 0) {

        //chama-se esta função para ela ir até Status Register e receber o status do keyboard que vai ser analisado (para deteção de erros)
        if (util_sys_inb(STAT_REG, &keyboard_status) != 0) return 1;

        //o input buffer está vazio (a 0), logo podemos escrever lá. Caso estivesse a 1 significa que estava cheio então tinhamos de esperar
        //que esses dados fossem processados para podermos escrever
        if ((keyboard_status & IN_BUFF_FULL) == 0) { 


            //se existir algo a escrever (verificado pela mascara de bits aplicada no "keyboard_status")
            //e iremos então escrever o "data" na determinada "port" com a ajuda de "sys_outb()" para que o controlador a receba
            if (sys_outb(port, data) != 0) return 1;

            return 0;
        }

        // Aguarda 30 ms para dar tempo ao KBC de processar os dados antes de tentar novamente
        tickdelay(micros_to_ticks(30000)); //deelay devido a lentidão do KBC para processar os dados
        attemps--;
    }


    return 1;
}
