/* SCUS_942.21 0x80019428..0x80019447. */
#include "psx/libspu.h"

/* Restart the SPU without resetting the voices (_SpuInit(1)). main_boot_reset_game_state calls it after a
 * reset or the ending. */
void SpuInitHot(void) {
    _SpuInit(1);
}
