#include "fft/world.h"
#include "psx/libgpu.h"

/* WORLD twin of battle_menu_init_semitransparent_sprt. */
void world_menu_init_semitransparent_sprt(SPRT* sprite) {
    SetSprt(sprite);
    SetShadeTex(sprite, 0);
    setRGB0(sprite, 0x80, 0x80, 0x80);
    sprite->x0 = 0x200;
    sprite->u0 = 0;
    sprite->v0 = 0;
    sprite->w = 0;
    sprite->h = 0;
    sprite->y0 = 0;
    sprite->clut = 0x7c3c;
    SetSemiTrans(sprite, 1);
}
