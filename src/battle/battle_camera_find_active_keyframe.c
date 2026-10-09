#include "fft/battle.h"
#include "psx/types.h"

/* Find the first camera keyframe of the channels in mask that is still ahead
 * of frame, reporting (table << 16 | index) and (target << 16 | end frame).
 *
 * The main table covers frames before phase1_duration; the for-each-target
 * table repeats every spawn_delay frames per hit target (current target, then
 * the next one), and once more for the last target within phase2_delay; the
 * cleanup table is relative to the end of that window.
 * Also known as find_active_camera_keyframe.
 *
 * The conditional-expression header reads keep the entry block from caching
 * the misc-data pointer. */
s32 battle_camera_find_active_keyframe(s16 frame, s32 mask, s32* out_index, s32* out_frame) {
    s32 phase1;
    s32 spawn_delay;
    s32 phase2_delay;
    s32 i;
    s32 target_index;
    s32 local_frame;
    s32 elapsed;
    s32 start;

    phase1 = g_battle_effect_timeline->phase1_duration;
    spawn_delay = g_battle_effect_coord_data.hit_counter != 0 ? g_battle_effect_timeline->spawn_delay : 0;
    phase2_delay = g_battle_effect_coord_data.hit_counter != 0 ? g_battle_effect_timeline->phase2_delay : 0;

    if (frame < phase1) {
        for (i = 0; i <= g_battle_effect_timeline->main.last_keyframe_index; i++) {
            if (frame < g_battle_effect_timeline->main.frame_thresholds[i]
                && (mask & g_battle_effect_timeline->main.selector[i]) != 0) {
                *out_index = i;
                *out_frame = g_battle_effect_timeline->main.frame_thresholds[i];
                return 1;
            }
        }
    }

    if (spawn_delay != 0 && frame < phase1 + (g_battle_effect_coord_data.hit_counter - 1) * spawn_delay) {
        target_index = 0;
        if (frame >= phase1) {
            elapsed = frame - phase1;
            target_index = elapsed / spawn_delay;
            local_frame = elapsed - target_index * spawn_delay;
        } else {
            local_frame = -1;
        }
        for (i = 0; i <= g_battle_effect_timeline->last_keyframe_index; i++) {
            if (g_battle_effect_timeline->frame_thresholds[i] >= spawn_delay) {
                break;
            }
            if (local_frame < g_battle_effect_timeline->frame_thresholds[i]
                && (mask & g_battle_effect_timeline->selector[i]) != 0) {
                *out_index = i + 0x10000;
                *out_frame = (target_index << 16)
                    + (g_battle_effect_timeline->frame_thresholds[i] + target_index * spawn_delay + phase1);
                return 1;
            }
        }
        target_index++;
        local_frame = -1;
        if (target_index < g_battle_effect_coord_data.hit_counter) {
            for (i = 0; i <= g_battle_effect_timeline->last_keyframe_index; i++) {
                if (g_battle_effect_timeline->frame_thresholds[i] >= spawn_delay) {
                    break;
                }
                if (local_frame < g_battle_effect_timeline->frame_thresholds[i]
                    && (mask & g_battle_effect_timeline->selector[i]) != 0) {
                    *out_index = i + 0x10000;
                    *out_frame = (target_index << 16)
                        + (g_battle_effect_timeline->frame_thresholds[i] + target_index * spawn_delay + phase1);
                    return 1;
                }
            }
        }
    }

    if (g_battle_effect_coord_data.hit_counter != 0) {
        target_index = g_battle_effect_coord_data.hit_counter - 1;
        start = phase1 + target_index * spawn_delay;
        if (frame < start + phase2_delay) {
            local_frame = -1;
            if (frame >= start) {
                local_frame = frame - phase1 - target_index * spawn_delay;
            }
            for (i = 0; i <= g_battle_effect_timeline->last_keyframe_index; i++) {
                if (local_frame < g_battle_effect_timeline->frame_thresholds[i]
                    && (mask & g_battle_effect_timeline->selector[i]) != 0) {
                    *out_index = i + 0x10000;
                    *out_frame = (target_index << 16)
                        + (g_battle_effect_timeline->frame_thresholds[i] + target_index * spawn_delay + phase1);
                    return 1;
                }
            }
        }
    }

    if (frame >= phase1 + (g_battle_effect_coord_data.hit_counter - 1) * spawn_delay + phase2_delay) {
        local_frame = frame - phase1 - (g_battle_effect_coord_data.hit_counter - 1) * spawn_delay - phase2_delay;
    } else {
        local_frame = -1;
    }
    for (i = 0; i <= g_battle_effect_timeline->cleanup.last_keyframe_index; i++) {
        if (local_frame < g_battle_effect_timeline->cleanup.frame_thresholds[i]
            && (mask & g_battle_effect_timeline->cleanup.selector[i]) != 0) {
            *out_index = i + 0x20000;
            *out_frame = g_battle_effect_timeline->cleanup.frame_thresholds[i] + phase1
                + (g_battle_effect_coord_data.hit_counter - 1) * spawn_delay + phase2_delay;
            return 1;
        }
    }
    return 0;
}
