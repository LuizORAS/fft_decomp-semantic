#include "fft/battle_action.h"
#include "psx/types.h"

/* Reflect: a reflectable ability not cast through Math Skill misses the target
 * and is recorded on it for the bounce. */
void action_check_reflect_reaction(void) {
    action_data_t* action;
    /* Loaded through a pointer so the reaction_id store stays above it. */
    u16* ability_ptr = &g_action_current_ability.ability_id;
    u16 ability;
    if (g_action_current_ability.skillset != SKILLSET_ID_MATH_SKILL
        && (g_action_current_ability.range_data.flags_3 & ABILITY_SECONDARY_FLAG_3_REFLECTABLE)) {
        action = g_action_target_data;
        action->reaction_id = ABILITY_ID_REACTION_REFLECT;
        ability = *ability_ptr;
        /* The target stores the accuracy as a 16-bit value (0x2a..0x2b). */
        action->attack_accuracy = 0;
        action->hit = 0;
        action->last_received_attack = ability;
        g_action_target_data->miss_type = ACTION_MISS_TYPE_REFLECTED;
    }
}
