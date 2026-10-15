#include "fft/battle.h"
#include "psx/types.h"

/* Release OPTION.OUT's AT list, which waits in option_menu_at_list_select_unit_thread while the
 * player looks at the unit picked from the list (battle_state_return_to_mini_menu). */
void battle_menu_end_at_list_unit_view(void) {
    g_battle_menu_at_list_unit_view_ended = 1;
}
