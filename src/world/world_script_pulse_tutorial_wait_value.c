#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* While a tutorial scripts the input, hold `value` in the tutorial wait
 * variable for one thread yield with every controller input disabled. */
void world_script_pulse_tutorial_wait_value(s32 value) {
    if (g_world_menu_input_mode == MENU_INPUT_TUTORIAL) {
        world_script_set_variable(EVENT_SCRIPT_VAR_TUTORIAL_WAIT_VALUE, value);
        world_script_set_variable(EVENT_SCRIPT_VAR_DISABLED_CONTROLLER_INPUTS, 0xFFFF);
        thread_yield();
        world_script_set_variable(EVENT_SCRIPT_VAR_TUTORIAL_WAIT_VALUE, 0);
    }
}
