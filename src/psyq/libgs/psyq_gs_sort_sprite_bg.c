#include "psx/libc.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* The ordering-table insertion helper shared by the sprite, line, box and
 * polygon sorters; its diagnostic at 0x800e0000 reads "ps_sort_sprite,bg: z
 * resolution overflow\n". It links the packet at `tag` into the OT bucket for
 * `z` and returns the address just past the packet, which the caller stores
 * as the new packet cursor.
 */

u32* psyq_gs_sort_sprite_bg(u32* tag, GsOT* ot, u16 z, s32 len) {
    u8 packet_length = len;
    u32* slot;
    s32 idx;

    idx = z - ot->offset;
    if (idx < 0) {
        printf(g_psyq_gs_sort_sprite_overflow_message);
    }
    slot = ot->org + idx;
    *tag = *slot;
    setlen(tag, packet_length);
    *slot = (u32)tag;
    setlen(slot, 0);
    return tag + (packet_length + 1);
}
