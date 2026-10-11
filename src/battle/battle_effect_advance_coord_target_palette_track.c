#include "fft/battle.h"
#include "psx/types.h"

/* Advance the target colour track of the parent timeline one frame: an APPLY
 * keyframe recolours every unit target of the effect.
 *
 * The frame timer and the loop counter are separate locals here: the target
 * keeps the timer in $v0 and the counter in $s1. */
void battle_effect_advance_coord_target_palette_track(
    battle_effect_palette_track_t* track, s16* keyframe, s16* remaining_frames) {
    s16 current;
    s32 index;
    s32 i;
    s32 timer;
    s32 preset;
    s16 red;
    s16 green;
    s16 blue;

    current = *keyframe;
    if (current < track->count - 1) {
        if (*remaining_frames == 0) {
            *keyframe = current + 1;
            index = current;
            timer = track->duration[index];
            if (timer != 0) {
                timer *= 8;
            } else {
                timer = 1;
            }
            *remaining_frames = timer;
            preset = track->preset[index];
            if (preset & EFFECT_COLOR_KEYFRAME_APPLY) {
                red = track->color[index][0];
                green = track->color[index][1];
                blue = track->color[index][2];
                for (i = 0; i < g_battle_effect_context.target_count; i++) {
                    if (g_battle_effect_context.targets.records[i].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                        battle_gfx_start_misc_unit_palette_modulation(preset & EFFECT_COLOR_KEYFRAME_MODE_MASK,
                            track->duration[index], g_battle_effect_context.targets.records[i].id.misc_id, red, green,
                            blue);
                    }
                }
            }
        }
        *remaining_frames = *remaining_frames - 1;
    }
}
