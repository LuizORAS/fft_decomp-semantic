#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Allocate text image VRAM for the sprite and give it the normal menu CLUT, GetClut(0x3c0, 0x1f0),
 * plus clut_offset sixteens of the clut word (4 is one VRAM line down). Callers pass 0 or 2; 2 carries out
 * of the x field: clut word 0x7c5c is GetClut(0x1c0, 0x1f1). */
void battle_text_configure_sprite_vram(
    RECT* rect, s16 width, s16 height, world_texture_prim_t* primitive, s32 clut_offset) {
    rect->w = width;
    rect->h = height;
    battle_gfx_alloc_tpage7_vram(rect, primitive, (u32*)-1);
    primitive->sprt.clut = (clut_offset << 4) + 0x7c3c;
}
