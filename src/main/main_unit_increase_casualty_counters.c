#include "fft/battle.h"
#include "fft/main.h"
#include "psx/types.h"

/* Count a unit lost for good, when its death counter runs out and it becomes a Crystal or Treasure
 * (battle_turn_run_clock): EVENT_SCRIPT_VAR_CASUALTIES counts units that started on the player's
 * team, EVENT_SCRIPT_VAR_INJURED the others; each stops at 9999. */
void main_unit_increase_casualty_counters(battle_stats_t* unit) {
    s32 id;
    s32 count;

    id = (unit->initial_team_flags & BATTLE_TEAM_MASK) != BATTLE_TEAM_BLUE ? EVENT_SCRIPT_VAR_INJURED
                                                                           : EVENT_SCRIPT_VAR_CASUALTIES;
    count = battle_script_get_variable(id);
    if (count < 0x270F) {
        count++;
    }
    battle_script_set_variable(id, count);
}
