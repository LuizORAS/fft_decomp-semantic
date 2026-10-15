#include "fft/wldcore.h"
#include "psx/types.h"

/* Put the context value display back as the world map keeps it: at its saved position, showing the play
 * time (loaded now), at HUD priority 1 and hidden (MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE). The two sound novel
 * menu levels call it; a sound novel shows the display in its own mode and place
 * (wldcore_sound_novel_show_value_display).
 *
 * Cache the flag and time source values before publishing their destinations; this preserves the target's
 * load/store schedule. */
void wldcore_gfx_reset_context_value_display(void) {
    u32 flags;
    s32 hours;
    s32 minutes;
    s32 seconds;

    g_wldcore_context_value_display_position = g_wldcore_saved_context_value_display_position;
    g_wldcore_context_value_display_mode = WLDCORE_VALUE_DISPLAY_PLAY_TIME;
    g_wldcore_hud_ot_priority = 1;
    flags = g_main_system_flags;
    hours = g_main_system_play_time_hours;
    minutes = g_main_system_play_time_minutes;
    seconds = g_main_system_play_time_seconds;
    g_main_system_flags = flags | MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
    g_wldcore_displayed_numeric_value = hours;
    g_wldcore_displayed_numeric_value_secondary = minutes;
    g_wldcore_displayed_numeric_value_tertiary = seconds;
}
