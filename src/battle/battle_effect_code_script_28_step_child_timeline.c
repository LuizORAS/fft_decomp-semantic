#include "fft/battle.h"
#include "psx/types.h"

/* The original update keeps the frame and tracks on one base; using the
 * embedded timeline alone changes offsets and scheduling. */
typedef struct battle_effect_child_timeline_view {
    s16 target_index;
    battle_unit_misc_halfword_t frame;
    battle_effect_child_timeline_state_t child;
} battle_effect_child_timeline_view_t;

/* Opcode 28: advance the record's child timeline by one frame, for the
 * record's target.
 *
 * The five particle channels run keyframe actions and spawn emitters, the
 * four colour tracks recolour the other units, the caster, the target and the
 * screen, and the three sound tracks play the effect's sounds; a root record
 * then starts the child camera table's moves and, under
 * EFFECT_FLAG_TIMING_CURVE_CHILD, the time scale follows the animation
 * timing curve. A target that reflected the effect instead starts the
 * reflect secondary effect, lasting spawn_delay frames, and jumps the
 * timeline to its end.
 *
 * The body inlines the logic of battle_effect_step_emitter_timeline and the
 * palette/background/sound track helpers. Each track keeps its own keyframe
 * local and the timers reuse i, as in those helpers: both reproduce the
 * target's load sharing and register allocation. */
s32 battle_effect_code_script_28_step_child_timeline(effect_record_t* record) {
    map_background_gradient_colors_t colors;
    battle_effect_secondary_init_t init;
    battle_effect_child_timeline_view_t* state;
    battle_effect_keyframe_table_t* table;
    battle_effect_palette_track_t* track;
    battle_effect_background_track_t* screen;
    battle_effect_tick_sound_track_t* sound;
    s32 i;
    s16 k;
    s16 units_keyframe;
    s16 caster_keyframe;
    s16 target_keyframe;
    s16 screen_keyframe;
    s16 sound_keyframe;
    s32 index;
    s32 preset;
    s32 sel;
    u16* actions;
    u16 slot;
    s16 step;
    s16 red;
    s16 green;
    s16 blue;
    u8 target;
    s32 mode;
    s32 kind;
    s32 sound_index;

    target = record->target_index;
    state = (battle_effect_child_timeline_view_t*)&record->target_index;
    if (g_battle_effect_context.targets.records[target].id.bytes[1] != BATTLE_EFFECT_HIT_REFLECTED) {
        for (i = 0; i < 5; i++) {
            table = &g_battle_effect_timing_channels->particle[i];
            if (state->child.remaining.tracks.particle[i] == 0) {
                if (state->child.keyframe.tracks.particle[i] == 0) {
                    state->child.remaining.tracks.particle[i] = table->frame_start[1] - table->frame_start[0];
                    state->child.step.tracks.particle[i] = 0;
                    state->child.keyframe.tracks.particle[i]++;
                    battle_effect_run_keyframe_actions(table->action[1], target, record);
                }
                while (state->child.remaining.tracks.particle[i] == 0) {
                    k = state->child.keyframe.tracks.particle[i];
                    state->child.keyframe.tracks.particle[i] = k + 1;
                    state->child.remaining.tracks.particle[i] = table->frame_start[k + 1] - table->frame_start[k];
                    state->child.step.tracks.particle[i] = 0;
                    battle_effect_run_keyframe_actions(table->action[k + 1], target, record);
                }
            }
            index = table->emitter[state->child.keyframe.tracks.particle[i]];
            if (index != 0) {
                step = state->child.step.tracks.particle[i]++;
                actions = table->action;
                sel = actions[state->child.keyframe.tracks.particle[i]] & EFFECT_KEYFRAME_ACTION_CALLBACK_MASK;
                if (sel != 0) {
                    slot = sel - 1;
                    if (state->child.remaining.tracks.particle[i] == 2) {
                        record->callback_phase[slot] = EFFECT_PHASE_DESTROY;
                    }
                    record->callbacks[slot](record->record_index, slot, index - 1, step);
                } else {
                    battle_effect_spawn_emitter_particles(record->record_index, step, index - 1, 0);
                }
            }
            state->child.remaining.tracks.particle[i]--;
        }

        track = &g_battle_effect_timing_channels->surroundings;
        units_keyframe = state->child.keyframe.tracks.color[0];
        if (units_keyframe < track->count - 1) {
            if (state->child.remaining.tracks.color[0] == 0) {
                state->child.keyframe.tracks.color[0] = units_keyframe + 1;
                index = units_keyframe;
                i = track->duration[index];
                if (i == 0) {
                    i = 1;
                } else {
                    i *= 8;
                }
                state->child.remaining.tracks.color[0] = i;
                preset = track->preset[index];
                if (preset & EFFECT_COLOR_KEYFRAME_APPLY) {
                    red = track->color[index][0];
                    green = track->color[index][1];
                    blue = track->color[index][2];
                    battle_map_color_field(
                        preset & EFFECT_COLOR_KEYFRAME_MODE_MASK, track->duration[index], red, green, blue);
                    for (i = 0; i < g_battle_effect_context.spectator_count; i++) {
                        battle_gfx_start_misc_unit_palette_modulation(preset & EFFECT_COLOR_KEYFRAME_MODE_MASK,
                            track->duration[index], g_battle_effect_palette_target_misc_ids[i], red, green, blue);
                    }
                    if (!(g_battle_map_weather_flags & BATTLE_MAP_WEATHER_FLAG_IGNORE_WEATHER)) {
                        battle_map_start_darkness_blend(preset & EFFECT_COLOR_KEYFRAME_MODE_MASK,
                            track->duration[index], red * 8, green * 8, blue * 8);
                    }
                }
            }
            state->child.remaining.tracks.color[0]--;
        }

        track = &g_battle_effect_timing_channels->caster;
        caster_keyframe = state->child.keyframe.tracks.color[1];
        if (caster_keyframe < track->count - 1) {
            if (state->child.remaining.tracks.color[1] == 0) {
                state->child.keyframe.tracks.color[1] = caster_keyframe + 1;
                index = caster_keyframe;
                i = track->duration[index];
                if (i == 0) {
                    i = 1;
                } else {
                    i *= 8;
                }
                state->child.remaining.tracks.color[1] = i;
                preset = track->preset[index];
                if (preset & EFFECT_COLOR_KEYFRAME_APPLY) {
                    red = track->color[index][0];
                    green = track->color[index][1];
                    blue = track->color[index][2];
                    if (g_battle_effect_context.targets.records[BATTLE_EFFECT_CASTER_ENTRY].target_type
                        == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                        battle_gfx_start_misc_unit_palette_modulation(preset & EFFECT_COLOR_KEYFRAME_MODE_MASK,
                            track->duration[index],
                            g_battle_effect_context.targets.records[BATTLE_EFFECT_CASTER_ENTRY].id.misc_id, red, green,
                            blue);
                    }
                }
            }
            state->child.remaining.tracks.color[1]--;
        }

        track = &g_battle_effect_timing_channels->target;
        target_keyframe = state->child.keyframe.tracks.color[2];
        if (target_keyframe < track->count - 1) {
            if (state->child.remaining.tracks.color[2] == 0) {
                state->child.keyframe.tracks.color[2] = target_keyframe + 1;
                index = target_keyframe;
                i = track->duration[index];
                if (i == 0) {
                    i = 1;
                } else {
                    i *= 8;
                }
                state->child.remaining.tracks.color[2] = i;
                preset = track->preset[index];
                if (preset & EFFECT_COLOR_KEYFRAME_APPLY) {
                    red = track->color[index][0];
                    green = track->color[index][1];
                    blue = track->color[index][2];
                    if (g_battle_effect_context.targets.records[target].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                        battle_gfx_start_misc_unit_palette_modulation(preset & EFFECT_COLOR_KEYFRAME_MODE_MASK,
                            track->duration[index], g_battle_effect_context.targets.records[target].id.misc_id, red,
                            green, blue);
                    }
                }
            }
            state->child.remaining.tracks.color[2]--;
        }

        screen = &g_battle_effect_timing_channels->screen;
        screen_keyframe = state->child.keyframe.tracks.color[3];
        if (screen_keyframe < screen->count - 1) {
            if (state->child.remaining.tracks.color[3] == 0) {
                state->child.keyframe.tracks.color[3] = screen_keyframe + 1;
                index = screen_keyframe;
                i = screen->duration[index];
                if (i == 0) {
                    i = 1;
                } else {
                    i *= 8;
                }
                state->child.remaining.tracks.color[3] = i;
                preset = screen->preset[index];
                mode = preset & EFFECT_COLOR_KEYFRAME_MODE_MASK;
                if (preset & EFFECT_COLOR_KEYFRAME_APPLY) {
                    red = screen->color[index][0] * 2;
                    green = screen->color[index][1] * 2;
                    blue = screen->color[index][2] * 2;
                    battle_map_modify_background_gradient(mode, screen->duration[index], red, green, blue);
                    if (g_battle_map_weather_flags & BATTLE_MAP_WEATHER_FLAG_IGNORE_WEATHER) {
                        battle_map_start_darkness_blend(mode, screen->duration[index], red, green, blue);
                    }
                } else {
                    colors.first.red = screen->color[index][0];
                    colors.first.green = screen->color[index][1];
                    colors.first.blue = screen->color[index][2];
                    colors.second.red = screen->second_color[index][0];
                    colors.second.green = screen->second_color[index][1];
                    colors.second.blue = screen->second_color[index][2];
                    battle_map_set_background_gradient(screen->duration[index], &colors);
                    if (g_battle_map_weather_flags & BATTLE_MAP_WEATHER_FLAG_IGNORE_WEATHER) {
                        battle_map_set_darkness_color(screen->duration[index], &colors.first);
                    }
                }
            }
            state->child.remaining.tracks.color[3]--;
        }

        for (i = 0; i < 3; i++) {
            sound = &g_battle_effect_timing_channels->sound[i];
            sound_keyframe = state->child.keyframe.tracks.sound[i];
            if (sound_keyframe < sound->count) {
                if (state->child.remaining.tracks.sound[i] == 0) {
                    state->child.keyframe.tracks.sound[i] = sound_keyframe + 1;
                    index = sound_keyframe;
                    state->child.remaining.tracks.sound[i] = sound->duration[index];
                    kind = sound->channel[index];
                    if (kind >= 2) {
                        sound_index = battle_effect_get_on_hit_sound_index(kind - 2);
                        if (sound_index != 0) {
                            SuzukiPlaySoundFindChannel(sound_index + g_battle_effect_sound_data_base);
                        }
                    }
                }
                state->child.remaining.tracks.sound[i]--;
            }
        }
        if (record->parent_index == 0) {
            battle_camera_start_effect_keyframe_moves(state->frame.s);
        }
        if (g_battle_effect_flags_section->flags & EFFECT_FLAG_TIMING_CURVE_CHILD) {
            battle_state_set_time_scale(battle_effect_get_animation_timing_curve_value(record));
        }
        state->frame.s++;
    } else {
        if (g_battle_effect_context.targets.records[target].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            init.targets.by_role.caster.record = g_battle_effect_context.targets.records[BATTLE_EFFECT_CASTER_ENTRY];
            init.targets.by_role.target[0].record = g_battle_effect_context.targets.records[target];
            battle_effect_init_secondary(
                BATTLE_EFFECT_SECONDARY_ANIMATION_REFLECT, g_battle_effect_timeline->spawn_delay, &init);
        }
        record->timeline_frame.u = g_battle_effect_timing_channels->duration;
    }
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
