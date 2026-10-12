#include "fft/battle.h"
#include "fft/battle_event.h"
#include "fft/battle_menu.h"
#include "psx/types.h"

/*
 * Animate the current window's first quad growing out from the rectangle
 * centre: each frame crops a sprite of the rectangle by the level's
 * percentage and copies the cropped bounds into the quad's corners.
 * WORLD twin: world_menu_animate_window_quad_crop.
 */
void battle_menu_animate_window_quad_crop(menu_window_buffer_t* buffers, RECT* rect, RECT* unused_to) {
    SPRT sprite;
    void* source; /* never assigned; the target passes an uninitialised $s5 */
    s32 level;
    u8 u = buffers->quads[0].u0;
    u8 v = buffers->quads[0].v0;
    s32 unused[2]; /* two unreferenced locals account for the 0x50 frame */

    for (level = 0; level < 12; level += g_battle_event_speed) {
        sprite.u0 = u;
        sprite.v0 = v;
        sprite.x0 = rect->x;
        sprite.y0 = rect->y;
        sprite.w = rect->w;
        sprite.h = rect->h;
        battle_menu_zoom_cursor_frame(source, &sprite, level);
        setXY4(&buffers[g_battle_menu_packet_buffer_index].quads[0], sprite.x0, sprite.y0, sprite.x0 + (sprite.w - 2),
            sprite.y0, sprite.x0, sprite.y0 + sprite.h, sprite.x0 + (sprite.w - 2), sprite.y0 + sprite.h);
        setUV4(&buffers[g_battle_menu_packet_buffer_index].quads[0], sprite.u0, sprite.v0, sprite.u0 + (sprite.w - 2),
            sprite.v0, sprite.u0, sprite.v0 + sprite.h, sprite.u0 + (sprite.w - 2), sprite.v0 + sprite.h);
        battle_thread_yield();
    }
}
