/* 0x8002230c: BIOS 0xa0:0x2f tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's rand (table A, 0x2f): advance x = x * 0x41c64e6d + 0x3039 and return bits 16-30 of x, 0 to
 * 0x7fff (psx-spx). The game's random rolls (the AI, formulas through battle_formula_get_random_0_7fff, unit
 * generation, encounters, propositions, effect particles) come from it. main_system_handle_vsync_callback
 * also calls it once a frame, so the sequence advances with time. */
int rand(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_RAND);
}
