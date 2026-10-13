#include "fft/battle_script.h"
#include "fft/battle_thread.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* WaitValue event instruction: yield until the script variable named by the
 * first 16-bit parameter reaches the value in the second. */
void script_wait_value_event_instruction(const event_wait_value_parameters_t* parameters) {
    s32 variable_id;
    s32 test_value;

    variable_id = script_read_s16(parameters->variable_id_le);
    test_value = script_read_s16(parameters->test_value_le);
    while (script_get_variable(variable_id) < test_value) {
        thread_yield();
    }
}
