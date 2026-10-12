#include "fft/battle_event.h"
#include "fft/battle_menu.h"

/* Thread body started from the ability list: build the AT list preview for the
 * highlighted ability, pulse tutorial wait value 0xfa and run it as a scrolling
 * list. */
void battle_menu_preview_ability_turn_at_list(void) {
    battle_menu_build_ability_preview_at_list();
    script_pulse_tutorial_wait_value(0xFA);
    battle_menu_run_scrolling_ability_list_thread();
}
