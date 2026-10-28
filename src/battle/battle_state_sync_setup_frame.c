#include "fft/battle.h"

/* Same as battle_state_sync_frame at a fixed one blank a frame, for the setup screens. */
s32 battle_state_sync_setup_frame(u32* ordering_table) {
    s32 sync_result;

    DrawSync(0);
    sync_result = VSync(0);
    PutDispEnv(&g_main_gfx_display_envs[g_main_gfx_screen_polarity]);
    PutDrawEnv(&g_main_gfx_draw_envs[g_main_gfx_screen_polarity]);
    DrawOTag(ordering_table);
    FntFlush(-1);
    return sync_result;
}
