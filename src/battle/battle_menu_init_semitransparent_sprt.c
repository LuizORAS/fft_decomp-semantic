#include "psx/libgpu.h"

/* Initialize the menu's spare sprite: a semi-transparent, shade-textured grey (0x80) SPRT with no
 * size, parked off-screen at x 0x200, with CLUT 0x7c3c. */
void battle_menu_init_semitransparent_sprt(SPRT* sprite) {
    SetSprt(sprite);
    SetShadeTex(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = 0x200;
    sprite->u0 = 0;
    sprite->v0 = 0;
    sprite->w = 0;
    sprite->h = 0;
    sprite->y0 = 0;
    sprite->clut = 0x7c3c;
    SetSemiTrans(sprite, 1);
}
