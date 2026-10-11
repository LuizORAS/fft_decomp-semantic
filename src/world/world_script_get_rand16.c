#include "fft/world.h"

/* Step the event script's 16-bit random generator,
 * seed = seed * 25173 + 13849 (mod 65536), and return the new seed. */
u32 world_script_get_rand16(void) {
    return g_world_event_random_seed = (g_world_event_random_seed * 25173U + 13849U) & 0xffffU;
}
