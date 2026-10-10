#include "fft/battle.h"
#include "psx/types.h"

/* Enter the event state and run the scenario check with no unit (0xff), as the
 * deployment screen does when it finishes. */
void battle_action_set_casting_unit_id_ff_and_init(void) {
    battle_action_set_casting_unit_id_ff();
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK, 0, 0xff, 0, 1);
}
