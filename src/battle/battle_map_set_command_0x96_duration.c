#include "fft/battle.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Start map weather effect mode 0x96 for `value` frames, from frame 0; effect
 * E458 passes the frames left of its timeline. An id other than 0x96 does
 * nothing. D_800F668C receives 0x95 and no code reads it. */
void battle_map_set_command_0x96_duration(s32 id, s32 value) {
    if (id == BATTLE_MAP_WEATHER_EFFECT_TIMED_PARTICLE_TRANSFORM) {
        D_800F668C = 0x95;
        g_battle_map_command_0x96_duration = value;
        g_battle_map_command_0x96_frame = 0;
        g_battle_map_weather_effect_mode = BATTLE_MAP_WEATHER_EFFECT_TIMED_PARTICLE_TRANSFORM;
    }
}
