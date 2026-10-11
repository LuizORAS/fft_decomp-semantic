#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Build the zodiac backdrop's primitive list: one textured quad per sprite
 * (g_world_gfx_zodiac_sprite_frames), shaded with the view colour and placed at the sprite's scaled
 * offset, followed by a black backdrop quad; the backdrop is always linked into the ordering table
 * and the quads only when the view asks for it. */
void world_gfx_build_zodiac_primitives(zodiac_draw_context_t* view, POLY_FT4* poly) {
    upoint16_t position;
    s32 i;
    s32 x;
    s32 y;

    for (i = 0; i < g_world_gfx_zodiac_sprite_count; i++) {
        SetPolyFT4(poly);
        poly->clut = GetClut(0, 0x1FD);
        poly->r0 = view->brightness;
        poly->g0 = view->brightness;
        poly->b0 = view->brightness;
        SetSemiTrans(poly, 1);
        SetShadeTex(poly, 0);
        x = g_world_gfx_zodiac_sprite_offsets[i].x;
        y = g_world_gfx_zodiac_sprite_offsets[i].y;
        x *= view->scale_x;
        y *= view->scale_y;
        if (x - (x / ONE) * ONE >= 0x800) {
            x += 1;
        }
        if (y - (y / ONE) * ONE >= 0x800) {
            y += 1;
        }
        x /= ONE;
        y /= ONE;
        position.x = x;
        position.y = y;
        world_gfx_set_scaled_poly_ft4_geometry_and_uv(poly, &g_world_gfx_zodiac_texture_page,
            (upoint16_t*)&g_world_gfx_full_texture_window, &g_world_gfx_zodiac_sprite_frames[i], &position,
            &view->offset);
        if (view->link_primitives != 0) {
            addPrim(view->ot, poly);
            poly++;
        }
    }
    /* The backdrop keeps the POLY_FT4 slot layout; only its tag is F4. */
    SetPolyF4((POLY_F4*)poly);
    poly->r0 = 8;
    poly->g0 = 8;
    poly->b0 = 8;
    SetSemiTrans(poly, 1);
    SetShadeTex(poly, 0);
    poly->x0 = 0;
    poly->y0 = 0;
    poly->x1 = 0xFF;
    poly->y1 = 0;
    poly->x2 = 0;
    poly->y2 = 0xFF;
    poly->x3 = 0xFF;
    poly->y3 = 0xFF;
    addPrim(view->ot, poly);
}
