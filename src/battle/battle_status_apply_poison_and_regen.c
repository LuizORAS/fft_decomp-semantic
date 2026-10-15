#include "fft/battle.h"
#include "psx/types.h"

/* Turn-clock HP upkeep for one unit: nothing for an absent, crystal, dead, jumping, petrified or
 * treasure unit (battle_status_check_crystal_dead_jump_petrify_treasure); otherwise Poison takes max
 * HP / 8, or without Poison, Regen restores max HP / 8, as the unit's action result. Returns the
 * result type (0 when neither applies). */
s32 battle_status_apply_poison_and_regen(battle_stats_t* unit) {
    u8 status;
    if (battle_status_check_crystal_dead_jump_petrify_treasure(unit) != 0) {
        return 0;
    }
    battle_action_set_target_unit(unit);
    status = unit->status_sets.current[3];
    if (status & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_POISON)) {
        g_battle_action_target_data->hp_damage = unit->max_hp >> 3;
        g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
    } else if (status & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_REGEN)) {
        g_battle_action_target_data->hp_healing = unit->max_hp >> 3;
        g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
    }
    return g_battle_action_target_data->attack_type;
}
