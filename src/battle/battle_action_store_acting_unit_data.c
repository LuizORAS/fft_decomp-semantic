#include "fft/battle_action.h"

/* Save the actor's place before its action, or end a reaction. A Mime gives back the weapons its
 * Mimic lent it (battle_action_prepare_mimic); another actor's facing goes to
 * g_battle_action_current_attacker in a primary action. A primary action saves the actor's x, y and
 * elevation (g_battle_acting_unit_x, _y, _elevation); a reaction restores the actor's command bytes from the
 * reaction snapshot (g_battle_action_saved_command) and returns to the primary context.
 *
 * The facing read goes through a raw 16-bit value so the lhu stays below the
 * preceding stores (struct-member loads hoist above scalar-global stores). */
void battle_action_store_acting_unit_data(battle_stats_t* unit) {
    if (unit->job_id == JOB_ID_MIME) {
        unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = ITEM_ID_NONE; /* right hand */
        unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = ITEM_ID_NONE;  /* left hand */
    } else if (g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        g_battle_action_current_attacker.facing = unit->position.facing;
    }

    if (g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        g_battle_acting_unit_x = unit->x;
        g_battle_acting_unit_y = unit->position.y;
        g_battle_acting_unit_elevation = unit->position.higher_elevation;
    } else {
        main_util_copy_action_data(&g_battle_action_saved_command, &unit->action_command);
        g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
    }
}
