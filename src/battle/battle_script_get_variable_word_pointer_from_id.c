#include "fft/battle_event.h"
#include "fft/battle_thread.h"

/* Resolve a script variable id to the 32-bit storage value that holds it. Ids below 0x80
 * own a 32-bit value; 0x80..0x35f pack 32 flags per 32-bit value starting at 32-bit variable 0x80, and
 * 0x360..0x3ff pack eight nibbles per 32-bit value starting at 32-bit variable 0x97. Ids from 0x400
 * are invalid and stop the calling thread; the result is unset on that path. */
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
