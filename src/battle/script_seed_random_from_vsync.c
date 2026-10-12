#include "fft/battle_script.h"

/* Seed the event script's 16-bit random generator (script_get_rand16) with
 * the vertical-blank count. */
void script_seed_random_from_vsync(void) {
    g_script_rand16_state = VSync(-1);
}
