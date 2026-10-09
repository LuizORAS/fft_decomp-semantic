#include "fft/battle.h"

s32* battle_script_get_variable_word_pointer_from_id(s32 id) {
    s32* result;
    s32 off;

    if (id < EVENT_SCRIPT_VAR_PACKED_BIT_FIRST) {
        result = (s32*)(id * 4 + (u32)g_battle_script_variables);
    } else if (id < EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) {
        off = ((id - EVENT_SCRIPT_VAR_PACKED_BIT_FIRST) / 32) * 4 + 0x200;
        result = (s32*)(off + (u32)g_battle_script_variables);
    } else if (id < EVENT_SCRIPT_VAR_END) {
        off = ((id - EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) / 8) * 4 + 0x25C;
        result = (s32*)(off + (u32)g_battle_script_variables);
    } else {
        battle_thread_exit_current();
    }
    return result;
}
