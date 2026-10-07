#include "fft/battle.h"
#include "psx/types.h"

/* Enter EVENT when the battle's setup is done (battle_state_run_battle_setup, map step 0xd) and let
 * system command 8 start the event that is due then, with no acting unit (0xff). */
void battle_state_enter_event_at_battle_start(void) {
    battle_state_enter_event();
    battle_menu_init_system_function(8, 0, 0xff, 0, 1);
}
