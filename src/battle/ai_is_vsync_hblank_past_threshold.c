#include "fft/battle_ai.h"
#include "psx/libetc.h"

/* 1 once 325 scanlines have passed since the last vertical blank: the
 * pathfinding loops use it to yield for the frame. */
s32 ai_is_vsync_hblank_past_threshold(void) {
    return VSync(1) >= AI_PATHFIND_SCANLINE_LIMIT;
}
