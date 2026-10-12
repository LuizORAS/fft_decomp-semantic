#include "psx/types.h"

/* WORLD twin of battle_menu_copy_bytes_to_halfwords. */
void world_menu_copy_bytes_to_halfwords(u16* destination, const u8* source, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        *destination = *source;
        source++;
        destination++;
    }
}
