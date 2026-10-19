#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Take the next of the 256 primitives of g_battle_gfx_sprite_prim_pool, used as a ring, and return its
 * index: the unit parts, shadows, cursor, tile glow and wait arrows are drawn with them. */
s16 battle_gfx_take_sprite_prim(void) {
    g_battle_gfx_sprite_prim_index = (g_battle_gfx_sprite_prim_index + 1) % 0x100;
    return g_battle_gfx_sprite_prim_index;
}
