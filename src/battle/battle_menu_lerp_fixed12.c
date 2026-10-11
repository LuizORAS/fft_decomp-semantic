#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Linear interpolation between start and end; factor is 20.12 fixed point (ONE
 * gives end). */
s32 battle_menu_lerp_fixed12(s32 start, s32 end, s32 factor) {
    return ((end - start) * factor) / ONE + start;
}
