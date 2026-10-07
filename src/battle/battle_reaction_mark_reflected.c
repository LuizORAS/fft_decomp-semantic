#include "fft/battle.h"
#include "psx/types.h"

/* Reflect the strike off the target: for a reflectable ability that is not a Math Skill, the target's
 * result becomes a reflected miss (accuracy 0) with reaction id ABILITY_ID_REACTION_REFLECT and the
 * ability in last_received_attack. battle_reaction_prepare_next later sends the spell on
 * (battle_target_apply_reflect). */
void battle_reaction_mark_reflected(void) {
    battle_action_data_t* action;
    /* Loaded through a pointer so the reaction_id store stays above it. */
    u16* ability_ptr = &g_current_ability.ability_id;
    u16 ability;
    if (g_current_ability.skillset != SKILLSET_ID_MATH_SKILL
        && (g_current_ability.range_data.flags_3 & ABILITY_SECONDARY_FLAG_3_REFLECTABLE)) {
        action = g_battle_action_target_data;
        action->reaction_id = ABILITY_ID_REACTION_REFLECT;
        ability = *ability_ptr;
        /* The target stores the accuracy as a halfword (0x2a..0x2b). */
        action->attack_accuracy = 0;
        action->hit = 0;
        action->last_received_attack = ability;
        g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_REFLECTED;
    }
}
