#include "fft/battle.h"
#include "psx/types.h"

/* Return g_main_system_vsync_interval. */
s32 battle_state_get_vsync_interval(void) {
    return g_main_system_vsync_interval;
}
