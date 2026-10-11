#include "fft/battle.h"
#include "psx/types.h"

/* Copies the 0x2a0 bytes of palettes from OTHER.SPR into the chicken, frog,
 * crystal and treasure palette tables, which lie one after the other (5, 5, 6 and
 * 5 CLUTs), setting the STP bit (0x80 of the high byte) on every colour except
 * colour 0 of each CLUT. */
void battle_gfx_copy_other_spr_palette_data_to_ram(u8* src) {
    s32 i = 0;
    s32 one = 1; /* held in a local: a literal changes the setup schedule */
    u8* dst = (u8*)&g_battle_gfx_chicken_palettes[0][0];
    do {
        u8 color_byte;
        if ((i & 1) && i % 32 != one) {
            color_byte = *src++ | 0x80;
        } else {
            color_byte = *src++;
        }
        *dst++ = color_byte;
        i++;
    } while (i < 0x2A0);
}
