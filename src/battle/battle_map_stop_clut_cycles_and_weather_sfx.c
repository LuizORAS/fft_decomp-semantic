#include "fft/battle_map.h"
#include "fft/sound.h"
#include "psx/types.h"

/* Stop the weather sound effects and every color rotation the 32 map texture
 * animations hold a handle for. Always returns 0. */
s32 battle_map_stop_clut_cycles_and_weather_sfx(void) {
    s32 index;

    battle_sound_stop_weather_sfx();

    for (index = 0; index < MAP_TEXTURE_ANIMATION_COUNT; index++) {
        if (g_battle_map_texture_animation_cycle_handles[index] != 0) {
            battle_map_stop_clut_cycle(g_battle_map_texture_animation_cycle_handles[index]);
            g_battle_map_texture_animation_cycle_handles[index] = 0;
        }
    }

    g_battle_sound_weather_sfx_enabled = 0;
    return 0;
}
