#include "fft/world.h"
#include "psx/types.h"

/* WaitValue event instruction: yield until the script variable named by the
 * first 16-bit parameter reaches the value in the second. */
void world_script_wait_value_event_instruction(const event_wait_value_parameters_t* parameters) {
    s32 variable_id = world_script_load_halfword(parameters->variable_id_le);
    s16 test_value = world_script_load_halfword(parameters->test_value_le);

    do {
        if (world_script_get_variable(variable_id) >= test_value) {
            break;
        }
        world_thread_yield();
    } while (1);
}
