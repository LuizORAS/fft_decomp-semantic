#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Enter COMMENCE_ATTACK_PHASE for a charged ability that comes due: copy the unit's stored
 * action (battle_stats_t +0x16e) into its command, take its ability id and start the charge
 * animation for a nonzero ability. */
void battle_state_enter_commence_attack_phase(void) {
    battle_stats_t* stats;
    battle_stats_t** stats_pointer;
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_COMMENCE_ATTACK_PHASE;
    unit = battle_unit_get_source_misc_data();
    stats_pointer = &unit->battle_data;
    stats = unit->battle_data;
    memcpy(&unit->command_state.ai.data.action, &stats->action_actor_id, 0x10);
    memcpy(&unit->command_state.ai.data.action.target_y, &stats->action_target_y, 4);
    /* last_ability_id is s16 in the header; the target loads it with lhu. */
    unit->used_ability_id = *(u16*)&(*stats_pointer)->last_ability_id;
    unit->ability_ct_resolved |= 2;
    if (unit->used_ability_id != 0) {
        battle_unit_start_ability_charge_animation_for_movement(unit);
    }
}
