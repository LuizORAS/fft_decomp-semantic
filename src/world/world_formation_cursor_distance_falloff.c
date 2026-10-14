#include "fft/world.h"
#include "psx/types.h"

/* Distance-based falloff: `base` minus the (y-stretched) distance from the
 * formation cursor to (x, y), clamped so it never drops below `floor`.
 * `dx` also holds the result. */
s32 world_formation_cursor_distance_falloff(s32 x, s32 y, s32 base, s32 floor) {
    s32 dx;
    s32 dy;

    dx = abs(g_world_formation_cursor_position.x - x);
    dx *= dx;
    dy = abs(g_world_formation_cursor_position.y - y);
    dy *= 2;
    dy *= dy;
    dx = base - (csqrt((dy + dx) * ONE) >> 12);
    if (dx < floor) {
        dx = floor;
    }
    return dx;
}
