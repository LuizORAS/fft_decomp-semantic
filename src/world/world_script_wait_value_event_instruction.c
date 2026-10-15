#include "fft/world.h"
#include "psx/types.h"

/* WaitValue event instruction: yield until the script variable named by the
 * first parameter halfword reaches the value in the second. */
void world_script_wait_value_event_instruction(const u8* parameters) {
    s32 variable_id = world_script_load_halfword(parameters);
    s16 test_value = world_script_load_halfword(parameters + 2);

    do {
        if (world_script_get_variable(variable_id) >= test_value) {
            break;
        }
        world_thread_yield();
    } while (1);
}
