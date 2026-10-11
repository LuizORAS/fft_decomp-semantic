#include "fft/battle.h"

/*
 * Dispatch one keyframe's action flags to every live effect target: bits 4-6
 * and 8-15 trigger unit reactions, and bit 3 restricts each of them to the
 * caster entry instead of the whole target list; bits 0-2 register a callback
 * slot.
 */
void battle_effect_run_keyframe_actions_all_targets(u16 flags, effect_record_t* state) {
    s32 misc_id;
    u16 animation;
    u16 slot;
    s32 i;

    if (flags & EFFECT_KEYFRAME_ACTION_FLAG_CASTER_ONLY) {
        misc_id = -1;
        if (g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            misc_id = g_battle_effect_context.targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.misc_id;
        }
    }
    if (flags & EFFECT_KEYFRAME_ACTION_FLAG_POST_ACTION_DISPLAY) {
        if (flags & EFFECT_KEYFRAME_ACTION_FLAG_CASTER_ONLY) {
            if (misc_id != -1) {
                battle_gfx_prepare_post_action_display_by_misc_id(misc_id);
            }
        } else {
            for (i = 0; i < g_battle_effect_context.target_count; i++) {
                if (g_battle_effect_context.targets[i].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                    battle_gfx_prepare_post_action_display_by_misc_id(
                        g_battle_effect_context.targets[i].data.unit.misc_id);
                }
            }
        }
    }
    if (flags & EFFECT_KEYFRAME_ACTION_FLAG_UPDATE_DISPLAY) {
        if (flags & EFFECT_KEYFRAME_ACTION_FLAG_CASTER_ONLY) {
            if (misc_id != -1) {
                battle_unit_update_display_by_misc_id(misc_id);
            }
        } else {
            for (i = 0; i < g_battle_effect_context.target_count; i++) {
                if (g_battle_effect_context.targets[i].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                    battle_unit_update_display_by_misc_id(g_battle_effect_context.targets[i].data.unit.misc_id);
                }
            }
        }
    }
    if (flags & EFFECT_KEYFRAME_ACTION_FLAG_TARGET_ANIMATION) {
        if (flags & EFFECT_KEYFRAME_ACTION_FLAG_CASTER_ONLY) {
            if (misc_id != -1) {
                battle_unit_set_target_anim_by_misc_id(misc_id);
            }
        } else {
            for (i = 0; i < g_battle_effect_context.target_count; i++) {
                if (g_battle_effect_context.targets[i].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                    battle_unit_set_target_anim_by_misc_id(g_battle_effect_context.targets[i].data.unit.misc_id);
                }
            }
        }
    }
    animation = (flags & EFFECT_KEYFRAME_ACTION_ANIMATION_MASK) >> EFFECT_KEYFRAME_ACTION_ANIMATION_SHIFT;
    if (animation != 0) {
        if (flags & EFFECT_KEYFRAME_ACTION_FLAG_CASTER_ONLY) {
            if (misc_id != -1) {
                battle_unit_set_specific_animation_by_misc_id(misc_id, animation);
            }
        } else {
            for (i = 0; i < g_battle_effect_context.target_count; i++) {
                if (g_battle_effect_context.targets[i].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
                    battle_unit_set_specific_animation_by_misc_id(
                        g_battle_effect_context.targets[i].data.unit.misc_id, animation);
                }
            }
        }
    }
    if (flags & EFFECT_KEYFRAME_ACTION_CALLBACK_MASK) {
        slot = (flags & EFFECT_KEYFRAME_ACTION_CALLBACK_MASK) - 1;
        state->callback_phase[slot] = EFFECT_PHASE_INIT;
        state->callbacks[slot] = g_battle_effect_callback_slots[slot];
    }
}
