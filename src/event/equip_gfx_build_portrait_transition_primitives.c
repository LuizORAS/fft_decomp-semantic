#include "fft/event_equip.h"
#include "fft/gfx.h"

/*
 * Build the primitives of the status banner's portrait cross-fade. EQUIP
 * copy of world_gfx_build_portrait_transition_primitives.
 *
 * *transition counts the steps: 0 shows the first portrait, 1 the second,
 * 2-3 capture both CLUTs, 4-9 wipe between the two portraits with two opaque
 * and two semi-transparent quads; from step 2 on it advances by the event
 * speed. `direction` 1 brings the first portrait in from the right as the
 * second leaves to the left; anything else mirrors that. `i` serves the
 * vertex loop, the CLUT loops and the wipe step alike.
 */
void equip_gfx_build_portrait_transition_primitives(const RECT* texture_rect, s32* transition,
    const s32* first_portrait, const s32* second_portrait, palette_t palettes[2], POLY_FT4* poly, s32 direction) {
    RECT second_rect;
    RECT first_rect;
    s32 i;

    battle_copy_bytes(&first_rect, texture_rect, sizeof(first_rect));
    battle_copy_bytes(&second_rect, texture_rect, sizeof(second_rect));
    second_rect.x += 0x10;

    for (i = 0; i < 4; i++) {
        setXY4(&poly[i], g_equip_gfx_portrait_origin.x + 2, g_equip_gfx_portrait_origin.y + 2,
            g_equip_gfx_portrait_origin.x + 0x21, g_equip_gfx_portrait_origin.y + 2, g_equip_gfx_portrait_origin.x + 2,
            g_equip_gfx_portrait_origin.y + 0x32, g_equip_gfx_portrait_origin.x + 0x21,
            g_equip_gfx_portrait_origin.y + 0x32);
    }

    if (*transition == 0) {
        equip_gfx_build_portrait_poly_ft4(*first_portrait, poly);
        return;
    }
    if (*transition == 1) {
        equip_gfx_build_portrait_poly_ft4(*second_portrait, poly);
        *transition += 1;
        return;
    }
    if (*transition < 4) {
        equip_gfx_build_portrait_poly_ft4(*second_portrait, poly);
        equip_gfx_set_clut_rect_from_id(&second_rect, poly->clut);
        poly++;
        equip_gfx_build_portrait_poly_ft4(*first_portrait, poly);
        equip_gfx_set_clut_rect_from_id(&first_rect, poly->clut);
        StoreImage(&first_rect, (u32*)palettes[0]);
        StoreImage(&second_rect, (u32*)palettes[1]);
        *transition += g_battle_event_speed;
    } else if (*transition < 10) {
        if (*transition == 4) {
            for (i = 0; i < 0x10; i++) {
                palettes[0][i].semi_transparent = 1;
            }
            for (i = 0; i < 0x10; i++) {
                palettes[1][i].semi_transparent = 1;
            }
            LoadImage(&first_rect, (u32*)palettes[0]);
            LoadImage(&second_rect, (u32*)palettes[1]);
        }

        i = *transition - 4;
        equip_gfx_build_portrait_poly_ft4(*second_portrait, poly);
        if (direction == 1) {
            gfx_clip_portrait_poly_from_right(poly, g_equip_gfx_portrait_transition_offsets[i]);
        } else {
            gfx_clip_portrait_poly_from_left(poly, g_equip_gfx_portrait_transition_offsets[i]);
        }
        poly++;

        equip_gfx_build_portrait_poly_ft4(*first_portrait, poly);
        if (direction == 1) {
            gfx_clip_portrait_poly_from_left(poly, 0x1f - g_equip_gfx_portrait_transition_offsets[i]);
        } else {
            gfx_clip_portrait_poly_from_right(poly, 0x1f - g_equip_gfx_portrait_transition_offsets[i]);
        }
        poly++;

        equip_gfx_build_portrait_poly_ft4(*second_portrait, poly);
        poly->tpage = (poly->tpage & 0xff9f) | 0x20;
        poly->clut = GetClut(second_rect.x, second_rect.y);
        SetShadeTex(poly, 0);
        SetSemiTrans(poly, 1);
        if (direction == 1) {
            gfx_clip_portrait_poly_from_right(poly, g_equip_gfx_portrait_transition_offsets[i + 8]);
        } else {
            gfx_clip_portrait_poly_from_left(poly, g_equip_gfx_portrait_transition_offsets[i + 8]);
        }
        poly++;

        equip_gfx_build_portrait_poly_ft4(*first_portrait, poly);
        poly->tpage = (poly->tpage & 0xff9f) | 0x20;
        poly->clut = GetClut(first_rect.x, first_rect.y);
        SetShadeTex(poly, 0);
        SetSemiTrans(poly, 1);
        if (direction == 1) {
            gfx_clip_portrait_poly_from_left(poly, 0x1f - g_equip_gfx_portrait_transition_offsets[i + 8]);
        } else {
            gfx_clip_portrait_poly_from_right(poly, 0x1f - g_equip_gfx_portrait_transition_offsets[i + 8]);
        }
        *transition += g_battle_event_speed;
    }
}
