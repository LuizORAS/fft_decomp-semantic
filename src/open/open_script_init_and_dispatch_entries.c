#include "fft/open.h"

typedef void (*open_script_entry_handler_t)(s16* entry);

extern open_script_entry_handler_t g_open_opcode_handlers[];

/* Run one frame of the ending script. Returns 0 once the script has ended and 1
 * while it runs.
 *
 * Updates the text scroll, the XA audio, the OPNTEX sequence and the picture
 * fade, then executes script entries. An entry is a row of 16-bit words at
 * data_base + byte_offset whose first word, the opcode, indexes
 * g_open_opcode_handlers. A handler advances byte_offset itself; one that has to
 * wait clears OPEN_SCRIPT_DISPATCH_CONTINUE to end the frame and is entered again
 * on the next, and opcode 0 clears OPEN_SCRIPT_DISPATCH_RUNNING. */
s32 open_script_init_and_dispatch_entries(void) {
    open_script_dispatch_state_t* state;
    s32* dispatch_arguments;
    open_script_entry_handler_t* handlers;
    s32 result;
    s16* entry;
    u32 flags;

    open_script_update_timing_and_record_values();
    open_script_update_xa_audio();
    open_gfx_update_opntex_sequence();
    open_script_update_screen_fade();

    state = &g_open_script_state.dispatch;
    result = state->flags | OPEN_SCRIPT_DISPATCH_CONTINUE;
    state->flags = result;
    if (result & OPEN_SCRIPT_DISPATCH_CONTINUE) {
        dispatch_arguments = &state->data_base;
        handlers = g_open_opcode_handlers;
        do {
            entry = (s16*)open_script_add_pointer_offset(dispatch_arguments[0], dispatch_arguments[1]);
            handlers[*entry](entry);
            /* Keep the data_base pointer live across the handler; a separate state base adds two instructions. */
            flags = ((open_script_dispatch_state_t*)((u8*)dispatch_arguments - (u32)
                         & ((open_script_dispatch_state_t*)0)->data_base))
                        ->flags;
            if ((flags & OPEN_SCRIPT_DISPATCH_RUNNING) == 0) {
                return 0;
            }
        } while (flags & OPEN_SCRIPT_DISPATCH_CONTINUE);
    }
    return 1;
}
