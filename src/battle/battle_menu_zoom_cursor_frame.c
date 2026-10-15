#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Scale a cursor sprite to g_battle_menu_cursor_zoom_percentages[step] percent about its centre,
 * keeping its position and texture coordinates even; steps from 12 leave it alone. `source` is not
 * read: the routine shares the (source, frame, step) shape of battle_gfx_copy_sprite_position_uv_and_size.
 *
 * Writing the half extent first in each sum (`w / 2 + x0`) is required; the reverse order reschedules
 * the reloads and adds a copy of `step`. */
void battle_menu_zoom_cursor_frame(const void* source, SPRT* sprite, s32 step) {
    s32 scale;
    s32 offset;
    if (step < 12) {
        scale = g_battle_menu_cursor_zoom_percentages[step];
        offset = sprite->w * scale / 200;
        sprite->x0 = (sprite->w / 2) + sprite->x0 - offset;
        sprite->u0 = (sprite->w / 2) + sprite->u0 - offset;
        sprite->x0 &= ~1;
        sprite->u0 &= ~1;
        offset = sprite->h * scale / 200;
        sprite->y0 = sprite->h / 2 + sprite->y0 - offset;
        sprite->w = sprite->w * scale / 100;
        sprite->v0 = sprite->h / 2 + sprite->v0 - offset;
        sprite->y0 &= ~1;
        sprite->v0 &= ~1;
        sprite->h = sprite->h * scale / 100;
        sprite->w &= 0xfe;
        sprite->h &= 0xfe;
    }
}
