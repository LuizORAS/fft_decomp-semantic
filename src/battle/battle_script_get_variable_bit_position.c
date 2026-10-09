#include "fft/battle.h"

s32 battle_script_get_variable_bit_position(s32 value) {
    s32 result;

    result = -1;
    if (value >= EVENT_SCRIPT_VAR_PACKED_BIT_FIRST) {
        if (value < EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) {
            result = value & 0x1f;
        } else if (value < EVENT_SCRIPT_VAR_END) {
            result = (value & 7) * 4;
        } else {
            battle_thread_exit_current();
        }
    }
    return result;
}
