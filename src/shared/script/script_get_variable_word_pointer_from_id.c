#include "fft/script.h"
#include "fft/thread.h"

/* Resolve a script variable id to the 32-bit storage value that holds it. Ids below 0x80 own a whole 32-bit
 * value; 0x80..0x35f pack 32 flags per 32-bit value starting at value 0x80, and 0x360..0x3ff pack eight nibbles per
 * 32-bit value starting at value 0x97. Ids from 0x400 are invalid and stop the calling thread; the result is unset on
 * that path.
 *
 * Port debt (QUIRKS.md): the addresses are computed through u32 casts. */
s32* script_get_variable_word_pointer_from_id(s32 variable_id) {
    s32* word;

    if (variable_id < EVENT_SCRIPT_VAR_PACKED_BIT_FIRST) {
        word = &g_script_variables[variable_id];
    } else if (variable_id < EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) {
        word = &g_script_variables[EVENT_SCRIPT_PACKED_BIT_WORD_FIRST
            + ((variable_id - EVENT_SCRIPT_VAR_PACKED_BIT_FIRST) / 32)];
    } else if (variable_id < EVENT_SCRIPT_VAR_END) {
        word = &g_script_variables[EVENT_SCRIPT_PACKED_NIBBLE_WORD_FIRST
            + ((variable_id - EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) / 8)];
    } else {
        thread_exit_current();
    }
    return word;
}
