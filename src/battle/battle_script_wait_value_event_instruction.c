#include "fft/battle.h"
#include "psx/types.h"

/* WaitValue event instruction: yield until the script variable named by the
 * first 16-bit parameter reaches the value in the second. */
void battle_script_wait_value_event_instruction(const event_wait_value_parameters_t* parameters) {
    s32 variable_id;
    s32 test_value;

    variable_id = battle_script_load_halfword(parameters->variable_id_le);
    test_value = battle_script_load_halfword(parameters->test_value_le);
    while (battle_script_get_variable(variable_id) < test_value) {
        battle_thread_yield();
    }
}
