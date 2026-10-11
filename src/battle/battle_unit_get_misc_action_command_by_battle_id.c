#include "fft/battle.h"
#include "psx/types.h"

/* The command action record (command_state.ai.data.action) of the unit in slot
 * battle_id, or 0 when it has no misc record. */
battle_action_command_t* battle_unit_get_misc_action_command_by_battle_id(u32 battle_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_battle_id(battle_id & 0xffff);

    if (unit == 0) {
        return 0;
    }
    return &unit->command_state.ai.data.action;
}
