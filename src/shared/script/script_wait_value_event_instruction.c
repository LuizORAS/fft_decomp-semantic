#include "fft/script.h"
#include "fft/thread.h"

/* Wait until the variable reaches the signed 16-bit test value. */
void script_wait_value_event_instruction(const event_wait_value_parameters_t* parameters) {
    s32 variable_id;
    s32 test_value;

    variable_id = script_read_s16(parameters->variable_id_le);
    test_value = script_read_s16(parameters->test_value_le);
    while (script_get_variable(variable_id) < test_value) {
        thread_yield();
    }
}
