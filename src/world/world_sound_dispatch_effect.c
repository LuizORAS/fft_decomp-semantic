#include "fft/world.h"
#include "psx/types.h"

/* Per-frame WORLD sound effect dispatch: stops the previous looping sound
 * when a stop id (0x73/0x2d) is queued, remembers ids 2/3 as the held loop,
 * and plays whichever id remains. */
void world_sound_dispatch_effect(void) {
    s32 sound_id;

    if (g_world_sound_release_held_loop != 0) {
        g_world_menu_sound_effect_id = MAIN_SFX_NONE;
    }
    if (g_world_sound_effect_id_to_play == MAIN_SFX_TEXT_GLYPH
        || g_world_sound_effect_id_to_play == MAIN_SFX_TEXT_PAGE) {
        main_sound_play_sfx_find_channel(g_world_sound_effect_id_to_play);
        g_world_sound_effect_id_to_play = MAIN_SFX_NONE;
    } else if (g_world_menu_sound_effect_id == MAIN_SFX_ERROR_MESSAGE) {
        main_sound_play_sfx_find_channel(MAIN_SFX_ERROR_MESSAGE);
        g_world_menu_sound_effect_id = MAIN_SFX_NONE;
    }
    if ((u32)(g_world_sound_effect_id_to_play - 2) < 2) {
        g_world_menu_sound_effect_id = g_world_sound_effect_id_to_play;
    }
    if (g_world_sound_effect_id_to_play < 0 || g_world_menu_sound_muted != 0) {
        g_world_sound_effect_id_to_play = MAIN_SFX_NONE;
    }
    sound_id = g_world_sound_effect_id_to_play;
    if (sound_id == MAIN_SFX_NONE) {
        sound_id = g_world_menu_sound_effect_id;
    }
    g_world_menu_sound_effect_id = MAIN_SFX_NONE;
    g_world_sound_effect_id_to_play = MAIN_SFX_NONE;
    if (sound_id != MAIN_SFX_NONE) {
        main_sound_play_sfx(sound_id);
    }
}
