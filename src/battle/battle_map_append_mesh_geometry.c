#include "fft/battle.h"
#include "psx/types.h"

/*
 * The geometry block starts with four polygon counts, then stores positions
 * for all four polygon classes, normals for textured polygons, per-polygon
 * render data, and finally terrain-tile locations for textured polygons.
 * Each position/normal vector occupies eight renderer bytes: XYZ followed by
 * a spare halfword.  The first position vector's spare halfword holds the
 * polygon's packed terrain-tile location.
 * Target 0x800f4dd4 is the geometry pass for primary and animated meshes;
 * battle_map_init_mesh_render_records performs the separate packet
 * pass. Count order is textured triangles/quads, then untextured
 * triangles/quads. The mesh metadata keeps the starting indices and the
 * incoming counts; the persistent totals advance at the end.
 */

void battle_map_append_mesh_geometry(void* geometry_data, battle_map_mesh_part_t* metadata) {
    u16* source = geometry_data;
    s32 i;
    s32 textured_triangle_count;
    s32 textured_quad_count;
    s32 untextured_triangle_count;
    s32 untextured_quad_count;

    g_battle_map_resource_load_state = 0x79;
    g_battle_map_mesh_data_buffer = (s32)g_battle_gfx_current_render_buffer;

    textured_triangle_count = *source++;
    textured_quad_count = *source++;
    untextured_triangle_count = *source++;
    untextured_quad_count = *source++;

    metadata->textured_triangle_start = g_battle_map_textured_triangle_count;
    metadata->textured_quad_start = g_battle_map_textured_quad_count;
    metadata->untextured_triangle_start = g_battle_map_untextured_triangle_count;
    metadata->untextured_quad_start = g_battle_map_untextured_quad_count;
    metadata->textured_triangle_count = textured_triangle_count;
    metadata->textured_quad_count = textured_quad_count;
    metadata->untextured_triangle_count = untextured_triangle_count;
    metadata->untextured_quad_count = untextured_quad_count;

    for (i = g_battle_map_textured_triangle_count; i < g_battle_map_textured_triangle_count + textured_triangle_count;
        i++) {
        g_battle_map_textured_triangle_positions[i].x0 = *source++;
        g_battle_map_textured_triangle_positions[i].y0 = *source++;
        g_battle_map_textured_triangle_positions[i].z0 = *source++;
        g_battle_map_textured_triangle_positions[i].x1 = *source++;
        g_battle_map_textured_triangle_positions[i].y1 = *source++;
        g_battle_map_textured_triangle_positions[i].z1 = *source++;
        g_battle_map_textured_triangle_positions[i].x2 = *source++;
        g_battle_map_textured_triangle_positions[i].y2 = *source++;
        g_battle_map_textured_triangle_positions[i].z2 = *source++;
    }

    for (i = g_battle_map_textured_quad_count; i < g_battle_map_textured_quad_count + textured_quad_count; i++) {
        g_battle_map_textured_quad_positions[i].x0 = *source++;
        g_battle_map_textured_quad_positions[i].y0 = *source++;
        g_battle_map_textured_quad_positions[i].z0 = *source++;
        g_battle_map_textured_quad_positions[i].x1 = *source++;
        g_battle_map_textured_quad_positions[i].y1 = *source++;
        g_battle_map_textured_quad_positions[i].z1 = *source++;
        g_battle_map_textured_quad_positions[i].x2 = *source++;
        g_battle_map_textured_quad_positions[i].y2 = *source++;
        g_battle_map_textured_quad_positions[i].z2 = *source++;
        g_battle_map_textured_quad_positions[i].x3 = *source++;
        g_battle_map_textured_quad_positions[i].y3 = *source++;
        g_battle_map_textured_quad_positions[i].z3 = *source++;
    }

    for (i = g_battle_map_untextured_triangle_count;
        i < g_battle_map_untextured_triangle_count + untextured_triangle_count; i++) {
        g_battle_map_untextured_triangle_positions[i].x0 = *source++;
        g_battle_map_untextured_triangle_positions[i].y0 = *source++;
        g_battle_map_untextured_triangle_positions[i].z0 = *source++;
        g_battle_map_untextured_triangle_positions[i].x1 = *source++;
        g_battle_map_untextured_triangle_positions[i].y1 = *source++;
        g_battle_map_untextured_triangle_positions[i].z1 = *source++;
        g_battle_map_untextured_triangle_positions[i].x2 = *source++;
        g_battle_map_untextured_triangle_positions[i].y2 = *source++;
        g_battle_map_untextured_triangle_positions[i].z2 = *source++;
    }

    for (i = g_battle_map_untextured_quad_count; i < g_battle_map_untextured_quad_count + untextured_quad_count; i++) {
        g_battle_map_untextured_quad_positions[i].x0 = *source++;
        g_battle_map_untextured_quad_positions[i].y0 = *source++;
        g_battle_map_untextured_quad_positions[i].z0 = *source++;
        g_battle_map_untextured_quad_positions[i].x1 = *source++;
        g_battle_map_untextured_quad_positions[i].y1 = *source++;
        g_battle_map_untextured_quad_positions[i].z1 = *source++;
        g_battle_map_untextured_quad_positions[i].x2 = *source++;
        g_battle_map_untextured_quad_positions[i].y2 = *source++;
        g_battle_map_untextured_quad_positions[i].z2 = *source++;
        g_battle_map_untextured_quad_positions[i].x3 = *source++;
        g_battle_map_untextured_quad_positions[i].y3 = *source++;
        g_battle_map_untextured_quad_positions[i].z3 = *source++;
    }

    for (i = g_battle_map_textured_triangle_count; i < g_battle_map_textured_triangle_count + textured_triangle_count;
        i++) {
        g_battle_map_textured_triangle_normals[i].vertex[0].vx = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[0].vy = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[0].vz = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[1].vx = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[1].vy = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[1].vz = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[2].vx = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[2].vy = *source++;
        g_battle_map_textured_triangle_normals[i].vertex[2].vz = *source++;
    }

    for (i = g_battle_map_textured_quad_count; i < g_battle_map_textured_quad_count + textured_quad_count; i++) {
        g_battle_map_textured_quad_normals[i].vertex[0].vx = *source++;
        g_battle_map_textured_quad_normals[i].vertex[0].vy = *source++;
        g_battle_map_textured_quad_normals[i].vertex[0].vz = *source++;
        g_battle_map_textured_quad_normals[i].vertex[1].vx = *source++;
        g_battle_map_textured_quad_normals[i].vertex[1].vy = *source++;
        g_battle_map_textured_quad_normals[i].vertex[1].vz = *source++;
        g_battle_map_textured_quad_normals[i].vertex[2].vx = *source++;
        g_battle_map_textured_quad_normals[i].vertex[2].vy = *source++;
        g_battle_map_textured_quad_normals[i].vertex[2].vz = *source++;
        g_battle_map_textured_quad_normals[i].vertex[3].vx = *source++;
        g_battle_map_textured_quad_normals[i].vertex[3].vy = *source++;
        g_battle_map_textured_quad_normals[i].vertex[3].vz = *source++;
    }

    /* Skip texture records (10/12 bytes) and untextured records (4 bytes). */
    source += textured_triangle_count * 5 + textured_quad_count * 6 + untextured_triangle_count * 2
        + untextured_quad_count * 2;

    for (i = g_battle_map_textured_triangle_count; i < g_battle_map_textured_triangle_count + textured_triangle_count;
        i++) {
        g_battle_map_textured_triangle_positions[i].terrain_tile.packed = *source++;
    }

    for (i = g_battle_map_textured_quad_count; i < g_battle_map_textured_quad_count + textured_quad_count; i++) {
        g_battle_map_textured_quad_positions[i].terrain_tile.packed = *source++;
    }

    g_battle_map_textured_triangle_count += textured_triangle_count;
    g_battle_map_textured_quad_count += textured_quad_count;
    g_battle_map_untextured_triangle_count += untextured_triangle_count;
    g_battle_map_untextured_quad_count += untextured_quad_count;
}
