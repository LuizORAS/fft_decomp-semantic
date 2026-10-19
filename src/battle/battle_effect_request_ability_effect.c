#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "fft/data.h"
#include "psx/types.h"

/* Request the effect of an ability: select its effect file, set the effect phase to REQUESTED, start the
 * charge secondary effect of its charge animation set, clear the sound channel counts and keep the init
 * record. Throw Ball takes the Fire, Ice or Bolt file for a fire, water or lightning element (another
 * element keeps the previous effect id), and ids from 0x200 continue the table after the action abilities.
 * An ability without an effect file requests nothing: the Break abilities
 * start the equip break secondary effect, and the Charge abilities and Accumulate the red orbs.
 *
 * `packed` carries the charge animation set in its low 16 bits and the element mask in its high 16 bits. */
void battle_effect_request_ability_effect(u32 packed, s32 ability, battle_effect_secondary_init_t* source) {
    g_battle_effect_load_state = BATTLE_EFFECT_LOAD_START;
    if (ability < ABILITY_ID_COUNT) {
        if (ability == ABILITY_ID_THROW_BALL) {
            s32 ball = battle_effect_get_element_index(packed >> 16);
            if (ball == BATTLE_EFFECT_ELEMENT_INDEX_FIRE)
                g_battle_ability_effect_id = BATTLE_EFFECT_FILE_FIRE;
            else if (ball == BATTLE_EFFECT_ELEMENT_INDEX_WATER)
                g_battle_ability_effect_id = BATTLE_EFFECT_FILE_ICE;
            else if (ball == BATTLE_EFFECT_ELEMENT_INDEX_LIGHTNING)
                g_battle_ability_effect_id = BATTLE_EFFECT_FILE_BOLT;
        } else {
            g_battle_ability_effect_id = g_battle_effect_ability_files[ability];
        }
    } else {
        /* Event ids (0x200 and up) continue the table after the action and reaction ability rows. */
        g_battle_ability_effect_id
            = g_battle_effect_ability_files[ability - ABILITY_ID_COUNT + ABILITY_ID_SUPPORT_FIRST];
    }

    if (g_battle_ability_effect_id >= 0) {
        g_battle_ability_effect_id &= BATTLE_EFFECT_ABILITY_FILE_MASK;
        g_battle_effect_playback_phase = BATTLE_EFFECT_PLAYBACK_PHASE_REQUESTED;
        if (g_battle_effect_secondary_by_charge_animation[packed & 0xffff] != 0) {
            g_battle_effect_charge_secondary_id = battle_effect_init_secondary(
                g_battle_effect_secondary_by_charge_animation[packed & 0xffff], packed >> 16, source);
        } else {
            g_battle_effect_charge_secondary_id = 0;
        }
        g_battle_effect_sound_call_counts[3] = 0;
        g_battle_effect_sound_call_counts[2] = 0;
        g_battle_effect_sound_call_counts[1] = 0;
        g_battle_effect_sound_call_counts[0] = 0;
        g_battle_effect_coord_data = *source;
        /* The coord data block retains a copy of the caller's secondary-init record. */
        D_801BC0D4 = 0;
        return;
    }

    if (ability >= ABILITY_ID_BATTLE_SKILL_HEAD_BREAK && ability <= ABILITY_ID_BATTLE_SKILL_MIND_BREAK)
        battle_effect_init_secondary(BATTLE_EFFECT_SECONDARY_ANIMATION_EQUIP_BREAK, 0, source);
    if ((ability >= ABILITY_ID_CHARGE_FIRST && ability < ABILITY_ID_MATH_FIRST)
        || ability == ABILITY_ID_BASIC_SKILL_ACCUMULATE)
        battle_effect_init_secondary(BATTLE_EFFECT_SECONDARY_ANIMATION_RED_ORBS, 0, source);
}
