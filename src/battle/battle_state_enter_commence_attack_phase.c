#include "fft/battle.h"
#include "fft/battle_camera.h"

/*
 * Copies the caster's action target data (unit_stats_t 0x16e..0x181) into
 * its runtime command state's action, then latches the used ability id.
 */
void battle_state_enter_commence_attack_phase(void) {
    unit_stats_t* stats;
    battle_unit_t* unit;

    camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_COMMENCE_ATTACK_PHASE;
    unit = battle_unit_get_source();
    stats = unit->stats;
    unit->command_state.ai.data.action = stats->action_command;
    /* action_command.ability_id is s16 in the header; the target loads it with lhu. */
    unit->used_ability_id = unit->stats->action_command.ability_id;
    unit->turn_action_flags |= BATTLE_UNIT_TURN_ACTION_ACTED;
    if (unit->used_ability_id != 0) {
        battle_unit_start_ability_charge_hold_animation(unit);
    }
}
