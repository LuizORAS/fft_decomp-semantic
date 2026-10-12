#include "fft/battle.h"
#include "psx/types.h"

/* The unit-ready test of the event script's wait instruction
 * (script_wait_for_unit_ready and its WORLD twin): it always passes. */
s32 battle_return_one(s32 unused_misc_id) {
    return 1;
}
