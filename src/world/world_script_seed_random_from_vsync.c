#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_seed_random_from_vsync: seed the event random generator
 * (world_script_get_rand16) with the vertical-blank count. */
void world_script_seed_random_from_vsync(void) {
    g_world_event_random_seed = VSync(-1);
}
