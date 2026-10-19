#include "fft/battle_map.h"
#include "psx/types.h"

/* Fill the current render buffer's packets for one mesh geometry block, from the polygons the loader
 * has already filled: the texture coordinates, CLUT and texture page of each textured triangle and quad,
 * and the colour of each untextured one. The loader runs it in each render buffer
 * (MAP_LOAD_STATE_MESH_RECORDS); battle_map_append_mesh_geometry has read the positions and normals of the
 * same block.
 *
 * The pass skips the position and normal vectors, then reads five 16-bit values per textured triangle and six
 * per textured quad. A CLUT keeps its low six bits (the CLUT) and gains the map CLUT row; texture
 * pages are copied as they are. Untextured POLY_F3/POLY_F4 packets start with colour 0/0/1. */
void battle_map_init_mesh_render_records(const map_mesh_geometry_t* geometry_data) {
    /* Named count loads fold the retail four 16-bit cursor advances. */
    const u16* source = &geometry_data->textured_triangle_count;
    s32 polygon_index;
    s32 textured_triangle_count;
    s32 textured_quad_count;
    s32 untextured_triangle_count;
    s32 untextured_quad_count;
    s32 triangle_vector_halfwords;
    s32 quad_vector_halfwords;
    u16 packed;
    s32 u0, v0, u1, v1, u2, v2, u3, v3;
    u16 clut_data;
    u16 texture_page;

    g_battle_map_mesh_last_render_buffer = g_battle_gfx_current_render_buffer;

    textured_triangle_count = *source++;
    textured_quad_count = *source++;
    untextured_triangle_count = *source++;
    untextured_quad_count = *source++;

    triangle_vector_halfwords = textured_triangle_count * 9;
    quad_vector_halfwords = textured_quad_count * 12;
    source += triangle_vector_halfwords + quad_vector_halfwords + untextured_triangle_count * 9
        + untextured_quad_count * 12 + triangle_vector_halfwords + quad_vector_halfwords;

    for (polygon_index = g_battle_map_textured_triangle_render_record_count;
        polygon_index < textured_triangle_count + g_battle_map_textured_triangle_render_record_count; polygon_index++) {
        packed = *source++;
        v0 = packed >> 8;
        u0 = packed & 0xff;
        packed = *source++;
        clut_data = packed;
        packed = *source++;
        v1 = packed >> 8;
        u1 = packed & 0xff;
        packed = *source++;
        texture_page = packed;
        packed = *source++;
        v2 = packed >> 8;
        u2 = packed & 0xff;
        g_battle_gfx_current_render_buffer->gt3[polygon_index].u0 = u0;
        g_battle_gfx_current_render_buffer->gt3[polygon_index].u1 = u1;
        g_battle_gfx_current_render_buffer->gt3[polygon_index].u2 = u2;
        g_battle_gfx_current_render_buffer->gt3[polygon_index].v0 = v0;
        g_battle_gfx_current_render_buffer->gt3[polygon_index].v1 = v1;
        g_battle_gfx_current_render_buffer->gt3[polygon_index].v2 = v2;
        g_battle_gfx_current_render_buffer->gt3[polygon_index].clut
            = (clut_data & 0x3f) | MAP_CLUT(MAP_CLUT_ROW_MAP, 0);
        g_battle_gfx_current_render_buffer->gt3[polygon_index].tpage = texture_page;
    }

    for (polygon_index = g_battle_map_textured_quad_render_record_count;
        polygon_index < textured_quad_count + g_battle_map_textured_quad_render_record_count; polygon_index++) {
        packed = *source++;
        v0 = packed >> 8;
        u0 = packed & 0xff;
        packed = *source++;
        clut_data = packed;
        packed = *source++;
        v1 = packed >> 8;
        u1 = packed & 0xff;
        packed = *source++;
        texture_page = packed;
        packed = *source++;
        v2 = packed >> 8;
        u2 = packed & 0xff;
        packed = *source++;
        v3 = packed >> 8;
        u3 = packed & 0xff;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].u0 = u0;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].u1 = u1;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].u2 = u2;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].u3 = u3;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].v0 = v0;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].v1 = v1;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].v2 = v2;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].v3 = v3;
        g_battle_gfx_current_render_buffer->gt4[polygon_index].clut
            = (clut_data & 0x3f) | MAP_CLUT(MAP_CLUT_ROW_MAP, 0);
        g_battle_gfx_current_render_buffer->gt4[polygon_index].tpage = texture_page;
    }

    for (polygon_index = g_battle_map_untextured_triangle_render_record_count;
        polygon_index < untextured_triangle_count + g_battle_map_untextured_triangle_render_record_count;
        polygon_index++) {
        g_battle_gfx_current_render_buffer->f3[polygon_index].r0 = 0;
        g_battle_gfx_current_render_buffer->f3[polygon_index].g0 = 0;
        g_battle_gfx_current_render_buffer->f3[polygon_index].b0 = 1;
    }

    for (polygon_index = g_battle_map_untextured_quad_render_record_count;
        polygon_index < untextured_quad_count + g_battle_map_untextured_quad_render_record_count; polygon_index++) {
        g_battle_gfx_current_render_buffer->f4[polygon_index].r0 = 0;
        g_battle_gfx_current_render_buffer->f4[polygon_index].g0 = 0;
        g_battle_gfx_current_render_buffer->f4[polygon_index].b0 = 1;
    }
}
