#include "fft/battle.h"
#include "psx/types.h"

/*
 * Save the actor's position or restore its pre-reaction action command.
 *
 * A Mime's weapons are cleared first. Normal actions save X/Y, elevation,
 * and facing; reactions restore the 20 saved command bytes at 0x16e.
 */
void battle_action_store_acting_unit_data(battle_stats_t* unit) {
    if (unit->job_id == JOB_ID_MIME) {
        unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = ITEM_ID_NONE; /* right hand */
        unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = ITEM_ID_NONE;  /* left hand */
    } else if (g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        g_battle_action_current_attacker.facing = unit->position.bits.facing;
    }

    if (g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        g_battle_acting_unit_x = unit->x;
        g_battle_acting_unit_y = unit->position.bits.y;
        g_battle_acting_unit_elevation = unit->position.raw >> 15;
    } else {
        main_util_copy_action_data(g_battle_action_saved_command, &unit->action_actor_id);
        g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
    }
}
