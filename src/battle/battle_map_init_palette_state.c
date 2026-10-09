#include "fft/battle.h"
#include "psx/types.h"

void battle_map_init_palette_state(void) {
    s32 clut_row;
    s32 enabled;
    s32 animation_index;

    g_battle_map_palette_state.upload_pending = 0;
    g_battle_map_palette_state.move_pending = 0;

    enabled = 1;
    clut_row = 0;
    do {
        g_battle_map_palette_state.clut_rows[clut_row].updates_disabled = 0;
        g_battle_map_palette_state.clut_rows[clut_row].enabled = enabled;

        animation_index = MAP_CLUTS_PER_ROW - 1;
        do {
            g_battle_map_palette_state.clut_rows[clut_row].animations[animation_index].active = 0;
            animation_index--;
        } while (animation_index >= 0);

        g_battle_map_palette_state.clut_rows[clut_row].components[0].red_delta_biased = 0;
        clut_row++;
    } while (clut_row < MAP_CLUT_ROW_COUNT);

    g_battle_map_background_gradient_transition.active = 0;
    g_battle_map_background_gradient_transition.repeat = MAP_COLOR_REPEAT_OFF;
    g_battle_map_ambient_light_transition.active = 0;
    g_battle_map_ambient_light_transition.repeat = MAP_COLOR_REPEAT_OFF;
    g_battle_map_palette_state.clut_rows[MAP_CLUT_ROW_SPRITES].enabled = 0;
    g_battle_map_palette_state.clut_rows[MAP_CLUT_ROW_UNITS].enabled = 0;
    g_battle_map_palette_state.clut_rows[MAP_CLUT_ROW_UNIT_SHADOWS].enabled = 0;
    g_battle_map_palette_state.clut_rows[MAP_CLUT_ROW_ACTION_RESULTS].enabled = 0;
    battle_map_reset_texture_animations();
}
