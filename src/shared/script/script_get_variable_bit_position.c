#include "fft/script.h"
#include "fft/thread.h"

/* Map a script variable id to the position of its field within its 32-bit storage value:
 * bit id & 31 for the one-bit flags (0x80..0x35f), bit (id & 7) * 4 for the
 * nibbles (0x360..0x3ff), and -1 for the 32-bit variables below 0x80. Ids
 * from 0x400 are invalid and stop the calling thread. */
s32 script_get_variable_bit_position(s32 value) {
    s32 result;

    result = -1;
    if (value >= EVENT_SCRIPT_VAR_PACKED_BIT_FIRST) {
        if (value < EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) {
            result = value & 0x1f;
        } else if (value < EVENT_SCRIPT_VAR_END) {
            result = (value & 7) * 4;
        } else {
            thread_exit_current();
        }
    }
    return result;
}
