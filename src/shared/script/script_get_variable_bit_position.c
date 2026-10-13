#include "fft/script.h"
#include "fft/thread.h"

/* Return the packed variable field position, or -1 for a full word; invalid ids exit the thread. */
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
