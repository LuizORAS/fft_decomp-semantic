#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

void world_text_configure_sprite_vram(RECT* rect, s32 width, s32 height, void* prim, s32 icon_index) {
    world_texture_prim_t* slot = prim;

    rect->w = width;
    rect->h = height;
    world_gfx_alloc_tpage7_vram(rect, slot, (u32*)-1);
    slot->sprt.clut = (icon_index * 0x10) + 0x7C3C;
}
