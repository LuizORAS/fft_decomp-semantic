#include "fft/script.h"
#include "psx/libetc.h"

/* Seed the event script's 16-bit random generator (script_get_rand16) with
 * the vertical-blank count. BATTLE and WORLD compile this text. */
void script_seed_random_from_vsync(void) {
    g_script_rand16_state = VSync(-1);
}
