#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "psx/libgpu.h"

/* Initialize an array of menu sprites with a shared CLUT; BATTLE and WORLD compile this text. */
void menu_init_sprite_array(SPRT* sprites, s32 count, s32 clut) {
    s32 index;

    for (index = 0; index < count; index++) {
        battle_menu_init_semitransparent_sprt(sprites);
        sprites->clut = clut;
        sprites++;
    }
}
