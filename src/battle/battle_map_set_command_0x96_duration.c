#include "fft/battle.h"
#include "psx/types.h"

/* Start map weather effect mode 0x96 for `value` frames, from frame 0; effect
 * E458 passes the frames left of its timeline. An id other than 0x96 does
 * nothing. D_800F668C receives 0x95 and no code reads it. */
void battle_map_set_command_0x96_duration(s32 id, s32 value) {
    if (id == 0x96) {
        D_800F668C = 0x95;
        g_battle_map_command_0x96_duration = value;
        g_battle_map_command_0x96_frame = 0;
        g_battle_map_weather_effect_mode = 0x96;
    }
}
