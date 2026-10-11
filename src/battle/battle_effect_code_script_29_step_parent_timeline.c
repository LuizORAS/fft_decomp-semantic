#include "fft/battle.h"
#include "psx/types.h"

/* The original update keeps the frame and tracks on one base; using the
 * embedded timeline alone changes offsets and scheduling. */
typedef struct battle_effect_parent_timeline_view {
    s16 target_index;
    battle_unit_misc_halfword_t frame;
    battle_effect_parent_timeline_state_t parent;
} battle_effect_parent_timeline_view_t;

/* Opcode 29: advance the record's parent timeline by one frame. The second
 * halfword is the script offset its child records start at.
 *
 * Before phase1_duration the phase 1 particle channels, colour tracks and
 * sound tracks run (and the time scale follows the timing curve under
 * EFFECT_FLAG_TIMING_CURVE_PHASE1); afterwards one child record per effect
 * target is started, spawn_delay frames apart. Phase 2 runs from
 * phase1_duration + (target_count - 1) * spawn_delay + child_duration. The
 * camera tracks advance every frame. */
s32 battle_effect_code_script_29_step_parent_timeline(effect_record_t* record) {
    battle_effect_parent_timeline_view_t* state;
    s32 i;
    s16 child;
    u8 unused_stack[16]; /* Unread locals that size the target's 0x48-byte frame. */

    state = (battle_effect_parent_timeline_view_t*)&record->target_index;
    if (state->frame.s >= g_battle_effect_timeline->phase1_duration) {
        while (
            state->parent.spawn_countdown == 0 && state->parent.spawned_count < g_battle_effect_context.target_count) {
            battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);

            child
                = battle_effect_start_script_record(record->script, instruction->immediate.value, record->record_index);
            g_effect_state_records[child].target_index = state->parent.spawned_count;
            state->parent.spawned_count++;
            state->parent.spawn_countdown += g_battle_effect_timeline->spawn_delay;
        }
        state->parent.spawn_countdown--;
    } else {
        for (i = 0; i < 5; i++) {
            battle_effect_step_emitter_timeline(record, &g_battle_effect_timeline->particle_channels[0][i],
                &state->parent.keyframe.tracks.particle[0][i], &state->parent.remaining.tracks.particle[0][i],
                &state->parent.step.tracks.particle[0][i]);
        }
        battle_effect_advance_target_palette_track(&g_battle_effect_timeline->color_tracks[0].surroundings,
            &state->parent.keyframe.tracks.track[0][0], &state->parent.remaining.tracks.track[0][0]);
        if (g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            battle_effect_advance_caster_palette_track(&g_battle_effect_timeline->color_tracks[0].caster,
                &state->parent.keyframe.tracks.track[1][0], &state->parent.remaining.tracks.track[1][0],
                g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].id.unit.misc_id);
        }
        battle_effect_advance_coord_target_palette_track(&g_battle_effect_timeline->color_tracks[0].target,
            &state->parent.keyframe.tracks.track[2][0], &state->parent.remaining.tracks.track[2][0]);
        battle_effect_advance_map_background_track(&g_battle_effect_timeline->color_tracks[0].screen,
            &state->parent.keyframe.tracks.track[3][0], &state->parent.remaining.tracks.track[3][0]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[0][0],
            &state->parent.keyframe.tracks.track[4][0], &state->parent.remaining.tracks.track[4][0],
            &state->parent.step.tracks._unknown_24[0][0][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[0][1],
            &state->parent.keyframe.tracks.track[5][0], &state->parent.remaining.tracks.track[5][0],
            &state->parent.step.tracks._unknown_24[1][0][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[0][2],
            &state->parent.keyframe.tracks.track[6][0], &state->parent.remaining.tracks.track[6][0],
            &state->parent.step.tracks._unknown_24[2][0][1]);
        if (g_battle_effect_flags_section->flags & EFFECT_FLAG_TIMING_CURVE_PHASE1) {
            battle_state_set_time_scale(battle_effect_get_timing_curve_value(record));
        }
    }
    i = g_battle_effect_timeline->phase1_duration
        + (g_battle_effect_context.target_count - 1) * g_battle_effect_timeline->spawn_delay
        + g_battle_effect_timeline->child_duration;
    if (state->frame.s >= i) {
        for (i = 0; i < 5; i++) {
            battle_effect_step_emitter_timeline(record, &g_battle_effect_timeline->particle_channels[1][i],
                &state->parent.keyframe.tracks.particle[1][i], &state->parent.remaining.tracks.particle[1][i],
                &state->parent.step.tracks.particle[1][i]);
        }
        battle_effect_advance_target_palette_track(&g_battle_effect_timeline->color_tracks[1].surroundings,
            &state->parent.keyframe.tracks.track[0][1], &state->parent.remaining.tracks.track[0][1]);
        if (g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            battle_effect_advance_caster_palette_track(&g_battle_effect_timeline->color_tracks[1].caster,
                &state->parent.keyframe.tracks.track[1][1], &state->parent.remaining.tracks.track[1][1],
                g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].id.unit.misc_id);
        }
        battle_effect_advance_coord_target_palette_track(&g_battle_effect_timeline->color_tracks[1].target,
            &state->parent.keyframe.tracks.track[2][1], &state->parent.remaining.tracks.track[2][1]);
        battle_effect_advance_map_background_track(&g_battle_effect_timeline->color_tracks[1].screen,
            &state->parent.keyframe.tracks.track[3][1], &state->parent.remaining.tracks.track[3][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[1][0],
            &state->parent.keyframe.tracks.track[4][1], &state->parent.remaining.tracks.track[4][1],
            &state->parent.step.tracks._unknown_24[0][1][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[1][1],
            &state->parent.keyframe.tracks.track[5][1], &state->parent.remaining.tracks.track[5][1],
            &state->parent.step.tracks._unknown_24[1][1][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[1][2],
            &state->parent.keyframe.tracks.track[6][1], &state->parent.remaining.tracks.track[6][1],
            &state->parent.step.tracks._unknown_24[2][1][1]);
    }
    battle_camera_advance_effect_tracks(state->frame.s);
    state->frame.s++;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
