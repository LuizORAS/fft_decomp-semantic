#include "fft/event_bunit.h"
#include "fft/gfx.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* The double-buffer binder uses a volatile view of these two capacities. */
extern u16 g_bunit_gfx_line_g2_capacity;
extern u16 g_bunit_gfx_line_g3_capacity;

/* Initializes every primitive pool of one frame buffer with its libgpu
 * constructor; polygons additionally get texture shading enabled.
 *
 * BUNIT twin of world_gfx_init_packet_pools. */
void bunit_gfx_init_packet_pools(bunit_menu_gfx_context_t* buffer) {
    s32 i;

    for (i = 0; i < g_bunit_gfx_poly_f3_capacity; i++) {
        SetPolyF3(&buffer->poly_f3[i]);
        SetShadeTex(&buffer->poly_f3[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_poly_ft3_capacity; i++) {
        SetPolyFT3(&buffer->poly_ft3[i]);
        SetShadeTex(&buffer->poly_ft3[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_poly_f4_capacity; i++) {
        SetPolyF4(&buffer->poly_f4[i]);
        SetShadeTex(&buffer->poly_f4[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_poly_ft4_capacity; i++) {
        SetPolyFT4(&buffer->textured_quads[i]);
        SetShadeTex(&buffer->textured_quads[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_poly_g3_capacity; i++) {
        SetPolyG3(&buffer->poly_g3[i]);
        SetShadeTex(&buffer->poly_g3[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_poly_gt3_capacity; i++) {
        SetPolyGT3(&buffer->poly_gt3[i]);
        SetShadeTex(&buffer->poly_gt3[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_poly_g4_capacity; i++) {
        SetPolyG4(&buffer->poly_g4[i]);
        SetShadeTex(&buffer->poly_g4[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_poly_gt4_capacity; i++) {
        SetPolyGT4(&buffer->poly_gt4[i]);
        SetShadeTex(&buffer->poly_gt4[i], 0);
    }
    for (i = 0; i < g_bunit_gfx_line_f2_capacity; i++) {
        SetLineF2(&buffer->lines[i]);
    }
    for (i = 0; i < g_bunit_gfx_line_f3_capacity; i++) {
        SetLineF3(&buffer->line_f3[i]);
    }
    for (i = 0; i < g_bunit_gfx_line_f4_capacity; i++) {
        SetLineF4(&buffer->line_f4[i]);
    }
    for (i = 0; i < g_bunit_gfx_line_g2_capacity; i++) {
        SetLineG2(&buffer->line_g2[i]);
    }
    for (i = 0; i < g_bunit_gfx_line_g3_capacity; i++) {
        SetLineG3(&buffer->line_g3[i]);
    }
    for (i = 0; i < g_bunit_gfx_line_g4_capacity; i++) {
        SetLineG4(&buffer->line_g4[i]);
    }
    for (i = 0; i < g_bunit_gfx_tile_capacity; i++) {
        SetTile(&buffer->tiles[i]);
    }
    for (i = 0; i < g_bunit_gfx_tile_1_capacity; i++) {
        SetTile1(&buffer->tiles_1[i]);
    }
    for (i = 0; i < g_bunit_gfx_tile_8_capacity; i++) {
        SetTile8(&buffer->tiles_8[i]);
    }
    for (i = 0; i < g_bunit_gfx_tile_16_capacity; i++) {
        SetTile16(&buffer->tiles_16[i]);
    }
    for (i = 0; i < g_bunit_gfx_sprite_capacity; i++) {
        SetSprt(&buffer->sprites[i]);
    }
    for (i = 0; i < g_bunit_gfx_sprite_8_capacity; i++) {
        SetSprt8(&buffer->sprites_8[i]);
    }
    for (i = 0; i < g_bunit_gfx_sprite_16_capacity; i++) {
        SetSprt16(&buffer->sprites_16[i]);
    }
    for (i = 0; i < g_bunit_gfx_draw_move_capacity; i++) {
        SetDrawMove(&buffer->moves[i]);
    }
}
