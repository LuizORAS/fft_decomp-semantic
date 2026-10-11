#include "psx/types.h"

s32 world_menu_copy_ff_terminated_bytes_to_halfwords(u16* destination, const u8* source) {
    s32 count = 0;

    for (;;) {
        if (*source == 0xFF) {
            return count;
        }
        *destination = *source;
        source++;
        count++;
        destination++;
    }
}
