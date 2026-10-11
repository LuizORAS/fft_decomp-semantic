#include "psx/libetc.h"

s32 CheckCallback(void) {
    return g_psyq_etc_interrupt_state.in_callback;
}
