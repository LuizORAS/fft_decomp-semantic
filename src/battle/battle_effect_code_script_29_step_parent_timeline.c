#include "fft/battle.h"
#include "psx/types.h"

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
    battle_effect_record_timeline_t* timeline;
    s32 i;
    s16 child;
    u8 unused_stack[16]; /* Unread locals that size the target's 0x48-byte frame. */

    timeline = &record->timeline;
    if (timeline->frame >= g_battle_effect_timeline->phase1_duration) {
        while (timeline->state.parent.spawn_countdown == 0
            && timeline->state.parent.spawned_count < g_battle_effect_context.target_count) {
            battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);

            child
                = battle_effect_start_script_record(record->script, instruction->immediate.value, record->record_index);
            g_effect_state_records[child].timeline.target_index = timeline->state.parent.spawned_count;
            timeline->state.parent.spawned_count++;
            timeline->state.parent.spawn_countdown += g_battle_effect_timeline->spawn_delay;
        }
        timeline->state.parent.spawn_countdown--;
    } else {
        for (i = 0; i < 5; i++) {
            battle_effect_step_emitter_timeline(record, &g_battle_effect_timeline->particle_channels[0][i],
                &timeline->state.parent.keyframe.tracks.particle[0][i],
                &timeline->state.parent.remaining.tracks.particle[0][i],
                &timeline->state.parent.step.tracks.particle[0][i]);
        }
        battle_effect_advance_target_palette_track(&g_battle_effect_timeline->color_tracks[0].surroundings,
            &timeline->state.parent.keyframe.tracks.track[0][0], &timeline->state.parent.remaining.tracks.track[0][0]);
        if (g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            battle_effect_advance_caster_palette_track(&g_battle_effect_timeline->color_tracks[0].caster,
                &timeline->state.parent.keyframe.tracks.track[1][0],
                &timeline->state.parent.remaining.tracks.track[1][0],
                g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id);
        }
        battle_effect_advance_coord_target_palette_track(&g_battle_effect_timeline->color_tracks[0].target,
            &timeline->state.parent.keyframe.tracks.track[2][0], &timeline->state.parent.remaining.tracks.track[2][0]);
        battle_effect_advance_map_background_track(&g_battle_effect_timeline->color_tracks[0].screen,
            &timeline->state.parent.keyframe.tracks.track[3][0], &timeline->state.parent.remaining.tracks.track[3][0]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[0][0],
            &timeline->state.parent.keyframe.tracks.track[4][0], &timeline->state.parent.remaining.tracks.track[4][0],
            &timeline->state.parent.step.tracks._unknown_24[0][0][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[0][1],
            &timeline->state.parent.keyframe.tracks.track[5][0], &timeline->state.parent.remaining.tracks.track[5][0],
            &timeline->state.parent.step.tracks._unknown_24[1][0][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[0][2],
            &timeline->state.parent.keyframe.tracks.track[6][0], &timeline->state.parent.remaining.tracks.track[6][0],
            &timeline->state.parent.step.tracks._unknown_24[2][0][1]);
        if (g_battle_effect_flags_section->flags & EFFECT_FLAG_TIMING_CURVE_PHASE1) {
            battle_state_set_time_scale(battle_effect_get_timing_curve_value(record));
        }
    }
    i = g_battle_effect_timeline->phase1_duration
        + (g_battle_effect_context.target_count - 1) * g_battle_effect_timeline->spawn_delay
        + g_battle_effect_timeline->child.duration;
    if (timeline->frame >= i) {
        for (i = 0; i < 5; i++) {
            battle_effect_step_emitter_timeline(record, &g_battle_effect_timeline->particle_channels[1][i],
                &timeline->state.parent.keyframe.tracks.particle[1][i],
                &timeline->state.parent.remaining.tracks.particle[1][i],
                &timeline->state.parent.step.tracks.particle[1][i]);
        }
        battle_effect_advance_target_palette_track(&g_battle_effect_timeline->color_tracks[1].surroundings,
            &timeline->state.parent.keyframe.tracks.track[0][1], &timeline->state.parent.remaining.tracks.track[0][1]);
        if (g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            battle_effect_advance_caster_palette_track(&g_battle_effect_timeline->color_tracks[1].caster,
                &timeline->state.parent.keyframe.tracks.track[1][1],
                &timeline->state.parent.remaining.tracks.track[1][1],
                g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id);
        }
        battle_effect_advance_coord_target_palette_track(&g_battle_effect_timeline->color_tracks[1].target,
            &timeline->state.parent.keyframe.tracks.track[2][1], &timeline->state.parent.remaining.tracks.track[2][1]);
        battle_effect_advance_map_background_track(&g_battle_effect_timeline->color_tracks[1].screen,
            &timeline->state.parent.keyframe.tracks.track[3][1], &timeline->state.parent.remaining.tracks.track[3][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[1][0],
            &timeline->state.parent.keyframe.tracks.track[4][1], &timeline->state.parent.remaining.tracks.track[4][1],
            &timeline->state.parent.step.tracks._unknown_24[0][1][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[1][1],
            &timeline->state.parent.keyframe.tracks.track[5][1], &timeline->state.parent.remaining.tracks.track[5][1],
            &timeline->state.parent.step.tracks._unknown_24[1][1][1]);
        battle_effect_update_on_hit_sound_timer(&g_battle_effect_timeline->sound_tracks[1][2],
            &timeline->state.parent.keyframe.tracks.track[6][1], &timeline->state.parent.remaining.tracks.track[6][1],
            &timeline->state.parent.step.tracks._unknown_24[2][1][1]);
    }
    battle_camera_advance_effect_tracks(timeline->frame);
    timeline->frame++;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
