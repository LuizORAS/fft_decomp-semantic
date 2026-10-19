#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Enter COMMENCE_ATTACK_PHASE for a charged ability that comes due: copy the unit's stored
 * action (battle_stats_t +0x16e) into its command, take its ability id and start the charge
 * animation for a nonzero ability. */
void battle_state_enter_commence_attack_phase(void) {
    battle_stats_t* stats;
    battle_unit_t* unit;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_COMMENCE_ATTACK_PHASE;
    unit = battle_unit_get_source();
    stats = unit->stats;
    unit->command_state.ai.data.action = stats->action_command;
    /* action_command.ability_id is s16 in the header; the target loads it with lhu. */
    unit->used_ability_id = unit->stats->action_command.ability_id;
    unit->turn_action_flags |= BATTLE_UNIT_TURN_ACTION_ACTED;
    if (unit->used_ability_id != 0) {
        battle_unit_start_ability_charging_animation(unit);
    }
}
