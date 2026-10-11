#include "fft/world.h"

/* Copy a six-word WORLD menu-controller state snapshot. */
void world_script_load_event_input_state(const menu_input_state_t* state) {
    g_world_menu_new_button_input = state->new_buttons;
    g_world_menu_current_button_input = state->current_buttons;
    g_world_menu_previous_button_input = state->previous_buttons;
    g_world_menu_input_initial_repeat_counter = state->initial_repeat_counter;
    g_world_menu_input_repeat_counter = state->repeat_counter;
    g_world_menu_input_secondary_repeat_counter = state->secondary_repeat_counter;
}
