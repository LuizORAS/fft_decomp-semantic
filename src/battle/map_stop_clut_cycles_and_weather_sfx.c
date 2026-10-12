#include "fft/battle_map.h"
#include "fft/battle_sound.h"
#include "psx/types.h"

/* Stop the weather sound effects and every colour rotation the 32 map texture
 * animations hold a handle for. Always returns 0. */
s32 map_stop_clut_cycles_and_weather_sfx(void) {
    s32 index;
    s32* handle;

    sound_stop_weather_sfx();

    index = 0;
    handle = g_map_texture_animation_cycle_handles;
    do {
        if (*handle != 0) {
            map_stop_clut_cycle(*handle);
            *handle = 0;
        }
        index++;
        handle++;
    } while (index < MAP_TEXTURE_ANIMATION_COUNT);

    g_sound_weather_sfx_enabled = 0;
    return 0;
}
