// Initializes peripherals, starts the game loop.

#include "game/hangman.h"
#include "peripherals/graphics/VBE.h"
#include "peripherals/i8254.h"
#include "peripherals/i8042.h"

int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}



