#include "fft/battle.h"
#include "psx/types.h"

/*
 * Select the effect file for an ability and arm its secondary animation.
 *
 * `packed` carries the ability instance in its low halfword and the sprite
 * palette in its high halfword. Ninja balls (0x189) pick their effect from the
 * thrown ball type. A negative table entry means the ability has no effect
 * file: only the two fixed secondary animations for the 0x8a and 0x196 ability
 * ranges are started, and nothing is retained.
 */
void battle_effect_set_ability_animation(u32 packed, s32 ability, battle_effect_secondary_init_t* source) {
    g_effect_load_state = 0;
    if (ability < ABILITY_ID_COUNT) {
        if (ability == ABILITY_ID_THROW_BALL) {
            s32 ball = battle_effect_get_ninja_ball(packed >> 16);
            if (ball == 1)
                g_battle_ability_effect_id = 0x10;
            else if (ball == 2)
                g_battle_ability_effect_id = 0x18;
            else if (ball == 8)
                g_battle_ability_effect_id = 0x14;
        } else {
            g_battle_ability_effect_id = g_battle_effect_ability_ids[ability];
        }
    } else {
        /* Ids from 0x200 up continue the table after the action abilities. */
        g_battle_ability_effect_id = g_battle_effect_ability_ids[ability - 0x200 + ABILITY_ID_SUPPORT_FIRST];
    }

    if (g_battle_ability_effect_id >= 0) {
        g_battle_ability_effect_id &= 0x1ff;
        g_battle_effect_phase = 1;
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
        /* The coord data block retains a copy of the caller's secondary-init record. */
        *(battle_effect_secondary_init_t*)&g_battle_effect_coord_data = *source;
        D_801BC0D4 = 0;
        return;
    }

    if ((u32)(ability - 0x8a) < 8)
        battle_effect_init_secondary(0x12, 0, source);
    if ((u32)(ability - ABILITY_ID_CHARGE_FIRST) < ABILITY_ID_MATH_FIRST - ABILITY_ID_CHARGE_FIRST
        || ability == ABILITY_ID_BASIC_SKILL_ACCUMULATE)
        battle_effect_init_secondary(0x11, 0, source);
}
