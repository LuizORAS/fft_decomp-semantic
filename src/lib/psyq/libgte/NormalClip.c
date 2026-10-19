#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Return the cross product of the three screen points `a`, `b` and `c` (NCLIP); its sign tells which way the
 * triangle faces. battle_gfx_build_cursor_tile_glow uses it. */
long NormalClip(long a, long b, long c) {
    long result;
    PSYQ_GTE_NORMALCLIP(a, b, c, result);
    return result;
}
