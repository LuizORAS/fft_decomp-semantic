#include "fft/world.h"
#include "psx/libgpu.h"

/* WORLD twin of battle_menu_init_sprite_array. */
void world_menu_init_sprite_array(SPRT* sprites, s32 count, s32 clut) {
    s32 index;

    for (index = 0; index < count; index++) {
        world_menu_init_semitransparent_sprt(sprites);
        sprites->clut = clut;
        sprites++;
    }
}
