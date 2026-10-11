#include "fft/battle.h"

/*
 * Dispatch one keyframe's action flags to a single effect target: bits 4-6
 * and 8-15 trigger unit reactions (bit 3 selects the caster entry), bits 0-2
 * register a callback slot.
 */
void battle_effect_run_keyframe_actions(u16 flags, u8 target, effect_record_t* state) {
    s32 misc_id;
    u32 animation;
    u16 slot;

    if (flags & EFFECT_KEYFRAME_ACTION_FLAG_CASTER_ONLY) {
        misc_id = -1;
        if (g_battle_effect_context.targets.records[BATTLE_EFFECT_CASTER_ENTRY].target_type
            == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            misc_id = g_battle_effect_context.targets.records[BATTLE_EFFECT_CASTER_ENTRY].id.misc_id;
        }
    } else {
        misc_id = -1;
        if (g_battle_effect_context.targets.records[target].target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT) {
            misc_id = g_battle_effect_context.targets.records[target].id.misc_id;
        }
    }
    if ((flags & EFFECT_KEYFRAME_ACTION_FLAG_POST_ACTION_DISPLAY) && misc_id != -1) {
        battle_gfx_prepare_post_action_display_by_misc_id(misc_id);
    }
    if ((flags & EFFECT_KEYFRAME_ACTION_FLAG_UPDATE_DISPLAY) && misc_id != -1) {
        battle_unit_update_display_by_misc_id(misc_id);
    }
    if ((flags & EFFECT_KEYFRAME_ACTION_FLAG_TARGET_ANIMATION) && misc_id != -1) {
        battle_unit_set_target_anim_by_misc_id(misc_id);
    }
    animation = (flags & EFFECT_KEYFRAME_ACTION_ANIMATION_MASK) >> EFFECT_KEYFRAME_ACTION_ANIMATION_SHIFT;
    if (animation != 0 && misc_id != -1) {
        battle_unit_set_specific_animation_by_misc_id(misc_id, animation);
    }
    if (flags & EFFECT_KEYFRAME_ACTION_CALLBACK_MASK) {
        slot = (flags & EFFECT_KEYFRAME_ACTION_CALLBACK_MASK) - 1;
        state->callback_phase[slot] = EFFECT_PHASE_INIT;
        state->callbacks[slot] = g_battle_effect_callback_slots[slot];
    }
}
