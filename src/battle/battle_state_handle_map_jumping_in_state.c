#include "fft/battle.h"

/* MAP_JUMPING_IN: update the units and brighten the screen by the transition step a frame;
 * once clear, return to 60 fps and the saved state. */
void battle_state_handle_map_jumping_in_state(void) {
    u32 intensity;
    s32 previous_state;
    char unused[24];

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
    g_battle_screen_fade_intensity -= *(u16*)&g_battle_state_map_transition_step;
    if ((s16)g_battle_screen_fade_intensity <= 0) {
        previous_state = g_previous_battle_game_state;
        g_battle_screen_fade_intensity = 0;
        g_previous_battle_game_state = 0;
        g_battle_state_vsync_interval = 1;
        g_battle_game_state = previous_state;
    }
}
