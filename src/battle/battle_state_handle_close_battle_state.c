#include "fft/battle.h"
#include "fft/battle_gfx.h"

/* CLOSE_BATTLE: update the units and darken the screen by the transition step.
 * battle_state_run_game_loop leaves its frame loop at the end of the frame that enters this
 * state, so no frame of that loop runs this handler. */
void battle_state_handle_close_battle_state(void) {
    u16 intensity;
    /* The original frame reserves this otherwise-unused stack area. */
    u8 stack_padding[0x18];

    battle_state_update_units();
    intensity = g_battle_screen_fade_intensity;
    if (intensity >= 0x100) {
        intensity = 0xff;
    }
    g_battle_screen_fade_polygons[g_main_gfx_screen_polarity].r0 = intensity;
    g_battle_screen_fade_polygons[g_main_gfx_screen_polarity].g0 = intensity;
    g_battle_screen_fade_polygons[g_main_gfx_screen_polarity].b0 = intensity;
    AddPrim(main_gfx_get_otag(), &g_battle_screen_fade_polygons[g_main_gfx_screen_polarity]);
    AddPrim(main_gfx_get_otag(), &g_battle_screen_fade_draw_modes[g_main_gfx_screen_polarity]);
    g_battle_screen_fade_intensity += g_battle_state_screen_fade_step;
}
