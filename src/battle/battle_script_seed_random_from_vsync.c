#include "fft/battle.h"
#include "psx/types.h"

/* Seed the event script's 16-bit random generator (battle_script_get_rand16) with
 * the vertical-blank count. */
void battle_script_seed_random_from_vsync(void) {
    g_battle_script_rand16_state = VSync(-1);
}
