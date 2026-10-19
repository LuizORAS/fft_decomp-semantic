#include "fft/world.h"
#include "psx/types.h"

/* Returns 1 when x and y both lie in 0..254. */
s32 world_formation_coords_fit_in_byte(s32 x, s32 y) {
    if (x >= 0 && x < 0xff) {
        return y >= 0 && y < 0xff;
    }
    return 0;
}
