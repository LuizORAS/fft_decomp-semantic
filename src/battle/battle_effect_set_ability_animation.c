#include "fft/battle.h"
#include "psx/types.h"

/*
 * Select the effect file for an ability and start its charge secondary effect.
 *
 * `packed` carries the ability's charge animation set in its low 16 bits and
 * its element mask in its high 16 bits. `ability` is an ability id or, from
 * ABILITY_ID_COUNT up, one of the effects past the abilities. A thrown ball
 * (ABILITY_ID_THROW_BALL) takes the Fire, Ice or Bolt effect by the ball's
 * element. A negative table entry means the ability has no effect file: only
 * the two fixed secondary animations for the equipment-break and Charge
 * ability ranges are started, and nothing is retained.
 */
void battle_effect_set_ability_animation(u32 packed, s32 ability, battle_effect_secondary_init_t* source) {
    g_battle_effect_load_state = BATTLE_EFFECT_LOAD_START;
    if (ability < ABILITY_ID_COUNT) {
        if (ability == ABILITY_ID_THROW_BALL) {
            s32 ball = battle_effect_get_ninja_ball(packed >> 16);
            if (ball == BATTLE_EFFECT_ELEMENT_INDEX_FIRE)
                g_battle_ability_effect_id = BATTLE_EFFECT_FILE_FIRE;
            else if (ball == BATTLE_EFFECT_ELEMENT_INDEX_WATER)
                g_battle_ability_effect_id = BATTLE_EFFECT_FILE_ICE;
            else if (ball == BATTLE_EFFECT_ELEMENT_INDEX_LIGHTNING)
                g_battle_ability_effect_id = BATTLE_EFFECT_FILE_BOLT;
        } else {
            g_battle_ability_effect_id = g_battle_effect_ability_ids[ability];
        }
    } else {
        /* Ids from 0x200 up continue the table after the action abilities. */
        g_battle_ability_effect_id = g_battle_effect_ability_ids[ability - ABILITY_ID_COUNT + ABILITY_ID_SUPPORT_FIRST];
    }

    if (g_battle_ability_effect_id >= 0) {
        g_battle_ability_effect_id &= BATTLE_EFFECT_ABILITY_FILE_MASK;
        g_battle_effect_phase = BATTLE_EFFECT_PHASE_REQUESTED;
        if (g_battle_effect_secondary_by_charge_animation[packed & 0xffff] != 0) {
            g_battle_effect_target_id = battle_effect_init_secondary(
                g_battle_effect_secondary_by_charge_animation[packed & 0xffff], packed >> 16, source);
        } else {
            g_battle_effect_target_id = 0;
        }
        g_battle_effect_sound_call_counts[3] = 0;
        g_battle_effect_sound_call_counts[2] = 0;
        g_battle_effect_sound_call_counts[1] = 0;
        g_battle_effect_sound_call_counts[0] = 0;
        g_battle_effect_context = *source;
        D_801BC0D4 = 0;
        return;
    }

    if (ability >= ABILITY_ID_BATTLE_SKILL_HEAD_BREAK && ability <= ABILITY_ID_BATTLE_SKILL_MIND_BREAK)
        battle_effect_init_secondary(BATTLE_EFFECT_SECONDARY_ANIMATION_EQUIP_BREAK, 0, source);
    if ((ability >= ABILITY_ID_CHARGE_FIRST && ability < ABILITY_ID_MATH_FIRST)
        || ability == ABILITY_ID_BASIC_SKILL_ACCUMULATE)
        battle_effect_init_secondary(BATTLE_EFFECT_SECONDARY_ANIMATION_RED_ORBS, 0, source);
}
