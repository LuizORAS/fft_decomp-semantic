#include "psx/libetc.h"
void psyq_etc_clear_vblank_callbacks(psyq_interrupt_callback_t* callbacks, s32 count) {
    s32 i;
    for (i = count - 1; i != -1; i--) {
        *callbacks++ = 0;
    }
}
