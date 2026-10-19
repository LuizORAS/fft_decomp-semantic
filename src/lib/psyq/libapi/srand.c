/* 0x8002231c: BIOS 0xa0:0x30 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's srand (table A, 0x30): set the rand state to `seed`. main_boot_run_startup seeds 1 at boot, and
 * world_unit_update_monster_breeding reseeds it from the play time before it rolls the monster eggs. */
void srand(unsigned int seed) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_SRAND);
}
