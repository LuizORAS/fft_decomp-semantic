#include "fft/battle.h"
#include "psx/types.h"

/* Copy a saved six-word input state (pressed, held and previous buttons and the
 * three repeat counters) into the event input globals. The BUNIT and CARD
 * overlays call it after computing the state themselves. */
void battle_script_load_event_input_state(const menu_input_state_t* saved) {
    g_battle_script_event_input = saved->new_buttons;
    g_battle_script_event_current_input = saved->current_buttons;
    g_battle_script_event_previous_input = saved->previous_buttons;
    g_battle_script_event_initial_repeat_counter = saved->initial_repeat_counter;
    g_battle_script_event_repeat_counter = saved->repeat_counter;
    g_battle_script_event_secondary_repeat_counter = saved->secondary_repeat_counter;
}
