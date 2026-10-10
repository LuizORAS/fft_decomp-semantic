#include "fft/battle.h"
#include "psx/types.h"

/*
 * Finds the first keyframe of the child camera table that ends after `frame`
 * and moves a track in `mask` (BATTLE_EFFECT_CAMERA_TRACK_*), reporting its
 * index and end frame. Nothing is found once the child timeline has run its
 * child_duration.
 *
 * The table pointer is re-read at the bottom of the loop, which is what stops
 * loop.c from turning the indexed reads into a single advancing pointer.
 */
s32 battle_effect_find_next_keyframe(s16 frame, s16 mask, s32* out_index, s32* out_frame) {
    effect_timeline_t* data;
    s32 i;

    data = g_battle_effect_timeline;
    if (frame < data->child_duration) {
        i = 0;
        if (data->last_keyframe_index >= 0) {
            do {
                if (frame < data->end_frame[i] && (mask & data->command[i]) != 0) {
                    *out_index = i;
                    *out_frame = g_battle_effect_timeline->end_frame[i];
                    return 1;
                }
                i++;
                data = g_battle_effect_timeline;
            } while (i <= data->last_keyframe_index);
        }
    }
    return 0;
}
