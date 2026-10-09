#include "fft/open.h"
#include "psx/types.h"

void open_opcode_wait_for_external_counter(s16* entry) {
    u32 flags;
    s32 counter;
    s32 limit;
    s32 offset;
    open_script_dispatch_state_t* state;

    state = &g_open_script_state.dispatch;
    flags = state->flags;
    entry++;
    if (flags & OPEN_SCRIPT_DISPATCH_WAITING) {
        limit = *entry;
        counter = state->external_counter;
        if (counter >= limit) {
            offset = state->byte_offset;
            state->flags = flags ^ OPEN_SCRIPT_DISPATCH_WAITING;
            state->byte_offset = offset + 4;
        } else {
            state->flags = flags ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
        }
    } else {
        state->flags = (flags | OPEN_SCRIPT_DISPATCH_WAITING) ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
        state->external_counter = 0;
    }
}
