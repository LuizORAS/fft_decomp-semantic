#include "psx/libgs.h"
#include "psx/types.h"

/* Emits a texture-page packet and SPRT primitive for the sprite, then links
 * the packet into the ordering table. */
void GsSortFastSprite(GsSPRITE* sp, GsOT* ot, u16 pri) {
    SPRT* packet;
    DR_TPAGE* draw_mode;
    GsOT_TAG* tag;
    u32 attr;
    u32 shade;
    u32 sprite_code;
    u32 mode; /* semi-transparency (attribute bit 30) in the code byte */
    u32 red;
    u8 green;
    u8 blue;
    u32 dr_mode; /* the DR_MODE packet code itself */
    u32 tpage_value;
    u16 sprite_y;
    u16 sprite_x;
    u32 packet_addr; /* the packet's address as the OT tag's 24-bit link field */
    s32 sum_x;
    s32 sum_y;

    attr = sp->attribute;
    if (!(attr & GsDOFF) && sp->w != 0 && sp->h != 0) {
        draw_mode = (DR_TPAGE*)GsOUT_PACKET_P;
        /* Share one DMA tag: the mode word occupies this primitive view's tag slot. */
        packet = (SPRT*)draw_mode->code;
        dr_mode = 0xE1000200;
        packet_addr = (u32)draw_mode & 0xFFFFFF;
        sprite_y = sp->y;
        sprite_x = sp->x;
        tpage_value = sp->tpage & 0x1F;
        draw_mode->code[0] = tpage_value | (((attr >> 17) & 0x180) | dr_mode) | ((attr >> 23) & 0x60);
        sum_x = sprite_x + *(u16*)&g_psyq_gs_offset_x;
        sum_y = sprite_y + *(u16*)&g_psyq_gs_offset_y;
        blue = sp->b;
        green = sp->g;
        red = sp->r;
        /* GPU command words retain the packed 32-bit stores. */
        *(u32*)&packet->x0 = (sum_x & 0xFFFF) | (sum_y << 16);
        mode = (attr >> 5) & 0x02000000;
        shade = (attr << 18) & 0x01000000;
        sprite_code = 0x64000000;
        *(u32*)&packet->r0 = mode | (shade | sprite_code) | (blue << 16) | (green << 8) | red;
        *(u32*)&packet->u0 = sp->u | (sp->v << 8) | (sp->cy << 22) | ((sp->cx << 12) & 0x3F0000);
        *(u32*)&packet->w = sp->w | (sp->h << 16);
        tag = &ot->org[pri] - ot->offset;
        /* Whole GPU tag transfers match; bitfield access adds masking and a read-modify-write. */
        draw_mode->tag = *(u32*)tag + 0x05000000;
        *(u32*)tag = packet_addr;
        GsOUT_PACKET_P = (PACKET*)(packet + 1);
    }
}
