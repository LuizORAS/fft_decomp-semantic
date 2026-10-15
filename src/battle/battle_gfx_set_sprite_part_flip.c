#include "psx/types.h"

/* Replace the two flip flags of a part of a sprite display. Nothing calls it. */
/* Replace the two mirror bits of a sprite part's flags (2 mirrors it in x, 4 in y). Never called. */
void battle_gfx_set_sprite_part_flip(u8* display, s32 part_index, u32 flags) {
    u8* part;

    part = display + part_index * 7;
    part[0x14] = (part[0x14] & 0xf9) | flags;
}
