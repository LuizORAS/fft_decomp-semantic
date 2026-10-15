#include "fft/battle.h"
#include "psx/types.h"

/* battle_gfx_weapon_shp_t with all of its 0x200 frame pointers. */
typedef struct battle_gfx_shp_frame_table {
    s16 first_frame[32];
    u8* frames[0x200];
} battle_gfx_shp_frame_table_t;

/*
 * Unpack a weapon or effect SHP file into dst: after four unread bytes, the
 * first frame of each of 32 weapon types, 0x200 frame offsets that become
 * pointers into the frame data (-1 maps to its start), and the frame data
 * itself, a halfword count and that many bytes, which is appended at
 * g_battle_gfx_shp_frame_data_cursor.
 *
 * The header loops read through `data` while the blob copy indexes `src`;
 * using one name for both loses the target's separate copy of the argument.
 */
void battle_gfx_copy_shp_data_to_ram(battle_gfx_shp_frame_table_t* dst, u8* src) {
    u32 i;
    u32 j;
    s32 offset;
    s32 value;
    u32 count;
    u8* word;
    u8* data;

    data = src;
    offset = 4;
    for (i = 0; i < 32; i++) {
        word = data + offset;
        dst->first_frame[i] = word[0] | (word[1] << 8);
        offset += 2;
    }
    for (i = 0; i < 0x200; i++) {
        word = data + offset;
        value = word[0] + (word[1] << 8) + (word[2] << 16) + (word[3] << 24);
        if (value == -1) {
            value = 0;
        }
        dst->frames[i] = g_battle_gfx_shp_frame_data_cursor + value;
        offset += 4;
    }
    word = data + offset;
    offset += 2;
    count = word[0] + (word[1] << 8);
    for (j = 0; j < count; j++) {
        g_battle_gfx_shp_frame_data_cursor[j] = src[offset++];
    }
    g_battle_gfx_shp_frame_data_cursor += j;
}
