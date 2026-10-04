#include "fft/battle.h"

/* Wait for the frame's vertical blanks, flip the display buffers and draw ordering_table. A
 * frame waits g_battle_state_vsync_interval blanks; during effects and action execution it
 * waits 2, or 4 and then 3 while g_battle_state_slowdown_frames counts down, or
 * g_battle_state_min_vsync_interval when longer. Returns VSync's timer-1 count. */
s32 battle_state_sync_frame(u32* ordering_table) {
    s32 wait;
    s32 sync_wait;
    s32 timer;
    s32 sync_result;

    if (g_battle_state_vsync_interval == 1) {
        DrawSync(0);
        sync_result = VSync(0);
    } else {
        DrawSync(0);
        if (g_battle_game_state == BATTLE_GAME_STATE_EFFECT
            || g_battle_game_state == BATTLE_GAME_STATE_ACTION_EXECUTE) {
            timer = g_battle_state_slowdown_frames;
            if (timer >= 0x10) {
                wait = 4;
                g_battle_state_slowdown_frames = timer - 1;
            } else if (timer != 0) {
                wait = 3;
                g_battle_state_slowdown_frames = timer - 1;
            } else {
                wait = 2;
            }
            sync_wait = g_battle_state_min_vsync_interval;
            if (wait < sync_wait) {
                if (sync_wait == 1) {
                    sync_wait = 0;
                }
            } else {
                sync_wait = wait;
            }
            sync_result = VSync(sync_wait);
        } else {
            sync_result = VSync(g_battle_state_vsync_interval);
        }
    }

    PutDispEnv(&g_main_gfx_display_envs[g_main_gfx_screen_polarity]);
    PutDrawEnv(&g_main_gfx_draw_envs[g_main_gfx_screen_polarity]);
    DrawOTag(ordering_table);
    FntFlush(-1);
    return sync_result;
}
