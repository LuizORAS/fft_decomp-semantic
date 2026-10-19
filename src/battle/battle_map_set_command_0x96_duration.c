#include "fft/battle.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Start weather mode 0x96 for `value` frames, from frame 0; effect E458 passes the frames left of its
 * timeline. In that mode the weather particles fall, slowing down, for the first eighth of the duration,
 * then rise while the field around the camera widens and sways, and at the end the event's weather is set
 * up again. An id other than 0x96 does nothing. D_800F668C receives 0x95 and no code reads it. */
void battle_map_set_command_0x96_duration(s32 id, s32 value) {
    if (id == MAP_WEATHER_EFFECT_TIMED_PARTICLE_TRANSFORM) {
        D_800F668C = 0x95;
        g_battle_map_command_0x96_duration = value;
        g_battle_map_command_0x96_frame = 0;
        g_battle_map_weather_effect_mode = MAP_WEATHER_EFFECT_TIMED_PARTICLE_TRANSFORM;
    }
}
