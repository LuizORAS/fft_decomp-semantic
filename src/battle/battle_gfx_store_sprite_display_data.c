#include "fft/battle.h"

/* clang-format off */
void battle_gfx_store_sprite_display_data(display, index, x_shift, y_shift, u, v, width, height, flags)
    battle_gfx_sprite_display_data_t* display;
    s32 index;
    s8 x_shift;
    s8 y_shift;
    u16 u;
    u16 v;
    u16 width;
    u16 height;
    u16 flags;
/* clang-format on */
{
    battle_gfx_sprite_part_display_data_t* part;

    part = &display->parts[index];
    part->x_shift = x_shift;
    part->y_shift = y_shift;
    part->u = u;
    part->v = v;
    part->width = width;
    part->height = height;
    part->flags = flags;
}
