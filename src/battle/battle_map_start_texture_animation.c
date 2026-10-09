#include "fft/battle.h"

s32 battle_map_start_texture_animation(s32 frame_duration, s32 polygon_group, s32 first_polygon, s32 last_polygon) {
    s32 index;
    map_texture_animation_state_t* state;

    for (index = 0; index < MAP_TEXTURE_ANIMATION_CAPACITY; index++) {
        state = &g_battle_map_texture_animation_states[index];
        if (state->active == 0) {
            state->active = 1;
            state->polygon_group = polygon_group;
            state->first_polygon = first_polygon;
            state->last_polygon = last_polygon;
            state->frame_duration = frame_duration;
            return index + 1;
        }
    }
    return 0;
}
