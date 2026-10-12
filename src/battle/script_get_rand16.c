#include "fft/battle_script.h"

/* Step the event script's random generator, state * 0x6255 + 0x3619 kept to 16
 * bits, and return the new state. script_run_variable_command calls it
 * when a variable opcode reads EVENT_SCRIPT_VAR_RANDOM_VALUE. */
s32 script_get_rand16(void) {
    return g_script_rand16_state = (g_script_rand16_state * 0x6255U + 0x3619U) & 0xffffU;
}
