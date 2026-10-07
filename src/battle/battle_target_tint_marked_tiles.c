#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Textured polygon positions (battle_map_append_mesh_geometry). terrain_tile
 * packs the tile's layer (bit 0), z (bits 1..7) and x (bits 8..15); 0xfffe
 * marks a polygon without a tile. */

/* Recolour the textured map polygons that lie on marked tiles (battle_target_tile_tint_e).
 *
 * MOVE_RANGE, ABILITY_RANGE and TARGETED tint the tiles carrying MAP_TILE_FLAG_MOVE_DESTINATION,
 * MAP_TILE_FLAG_ABILITY_RANGE or MAP_TILE_FLAG_TARGETED and select CLUT row 0x1e1 (the move range in
 * blue); the CLEAR_* modes restore CLUT row 0x1e0 and the ambient colour
 * g_battle_map_ambient_polygon_color (keeping polygons whose flag bit 15 is set), and RESET resets
 * every CLUT row. g_battle_target_tile_mark_modes records the state of the three marks; when the
 * other two are both clear, or in their restored states, the tint modes also shift the unit
 * palettes. A polygon's packed terrain tile (layer bit 0, z bits 1..7, x bits 8..15; 0xfffe for
 * none) selects its map tile.
 *
 * The CLUT and tint stores index the render buffer's polygon arrays; the ambient colour stores use
 * pointer arithmetic on the decayed array (`g_battle_data->gt3 + i`) instead, which is what keeps
 * the target's `addu offset, base` operand order for them.
 *
 * Every caller also passes a second argument, which the routine never reads. */
void battle_target_tint_marked_tiles(s32 mode, s32 unused) {
    s32 i;
    s16 other_state_a;
    s16 other_state_b;
    s16 tile;
    u8 marks;

    g_battle_map_tile_data_ptr = g_battle_map_tile_data;
    switch (mode) {
    case BATTLE_TARGET_TINT_RESET:
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[0]; i++) {
            g_battle_data->gt3[i].clut = (g_battle_data->gt3[i].clut & 0x803f) | 0x7800;
        }
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[1]; i++) {
            g_battle_data->gt4[i].clut = (g_battle_data->gt4[i].clut & 0x803f) | 0x7800;
        }
        g_battle_target_tile_mark_modes[0] = BATTLE_TARGET_TINT_CLEAR_MOVE_RANGE;
        g_battle_target_tile_mark_modes[1] = BATTLE_TARGET_TINT_CLEAR_ABILITY_RANGE;
        g_battle_target_tile_mark_modes[2] = BATTLE_TARGET_TINT_CLEAR_TARGETED;
        battle_map_polygon_flag_command(0x46);
        battle_map_polygon_flag_command(0x94);
        g_battle_target_tile_color_buffer = g_battle_data;
        break;
    case BATTLE_TARGET_TINT_MOVE_RANGE:
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[0]; i++) {
            if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_MOVE_DESTINATION) {
                    g_battle_data->gt3[i].clut = (g_battle_data->gt3[i].clut & 0x803f) | 0x7840;
                    g_battle_data->gt3[i].r0 = 0x20;
                    g_battle_data->gt3[i].r1 = 0x20;
                    g_battle_data->gt3[i].r2 = 0x20;
                    g_battle_data->gt3[i].g0 = 0x28;
                    g_battle_data->gt3[i].g1 = 0x28;
                    g_battle_data->gt3[i].g2 = 0x28;
                    g_battle_data->gt3[i].b0 = 0x60;
                    g_battle_data->gt3[i].b1 = 0x60;
                    g_battle_data->gt3[i].b2 = 0x60;
                    g_battle_map_textured_triangle_positions[i].polygon_flags &= 0xfffe;
                }
            }
        }
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[1]; i++) {
            if (g_battle_map_textured_quad_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_quad_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_MOVE_DESTINATION) {
                    g_battle_data->gt4[i].clut = (g_battle_data->gt4[i].clut & 0x803f) | 0x7840;
                    g_battle_data->gt4[i].r0 = 0x20;
                    g_battle_data->gt4[i].r1 = 0x20;
                    g_battle_data->gt4[i].r2 = 0x20;
                    g_battle_data->gt4[i].r3 = 0x20;
                    g_battle_data->gt4[i].g0 = 0x28;
                    g_battle_data->gt4[i].g1 = 0x28;
                    g_battle_data->gt4[i].g2 = 0x28;
                    g_battle_data->gt4[i].g3 = 0x28;
                    g_battle_data->gt4[i].b0 = 0x60;
                    g_battle_data->gt4[i].b1 = 0x60;
                    g_battle_data->gt4[i].b2 = 0x60;
                    g_battle_data->gt4[i].b3 = 0x60;
                    g_battle_map_textured_quad_positions[i].polygon_flags &= 0xfffe;
                }
            }
        }
        g_battle_target_tile_mark_modes[0] = BATTLE_TARGET_TINT_MOVE_RANGE;
        other_state_a = g_battle_target_tile_mark_modes[1];
        other_state_b = g_battle_target_tile_mark_modes[2];
        if ((other_state_a | other_state_b) == 0 || (other_state_a == 3 && other_state_b == 8)) {
            battle_map_modify_palette(9, 8, 1, 0, 1, 10, 10, 10);
        }
        break;
    case BATTLE_TARGET_TINT_CLEAR_MOVE_RANGE:
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[0]; i++) {
            if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_MOVE_DESTINATION) {
                    g_battle_data->gt3[i].clut = (g_battle_data->gt3[i].clut & 0x803f) | 0x7800;
                    if (g_battle_map_textured_triangle_positions[i].polygon_flags & 0x8000) {
                        g_battle_map_textured_triangle_positions[i].polygon_flags |= 1;
                    } else {
                        (g_battle_data->gt3 + i)->r0 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt3 + i)->g0 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt3 + i)->b0 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt3 + i)->r1 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt3 + i)->g1 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt3 + i)->b1 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt3 + i)->r2 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt3 + i)->g2 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt3 + i)->b2 = g_battle_map_ambient_polygon_color[2];
                    }
                }
            }
        }
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[1]; i++) {
            if (g_battle_map_textured_quad_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_quad_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_MOVE_DESTINATION) {
                    g_battle_data->gt4[i].clut = (g_battle_data->gt4[i].clut & 0x803f) | 0x7800;
                    if (g_battle_map_textured_quad_positions[i].polygon_flags & 0x8000) {
                        g_battle_map_textured_quad_positions[i].polygon_flags |= 1;
                    } else {
                        (g_battle_data->gt4 + i)->r0 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g0 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b0 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt4 + i)->r1 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g1 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b1 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt4 + i)->r2 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g2 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b2 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt4 + i)->r3 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g3 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b3 = g_battle_map_ambient_polygon_color[2];
                    }
                }
            }
        }
        g_battle_target_tile_mark_modes[0] = BATTLE_TARGET_TINT_CLEAR_MOVE_RANGE;
        g_battle_target_tile_color_buffer = g_battle_data;
        break;
    case BATTLE_TARGET_TINT_ABILITY_RANGE:
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[0]; i++) {
            if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_ABILITY_RANGE) {
                    g_battle_data->gt3[i].clut = (g_battle_data->gt3[i].clut & 0x803f) | 0x7840;
                    g_battle_data->gt3[i].r0 = 0x60;
                    g_battle_data->gt3[i].r1 = 0x60;
                    g_battle_data->gt3[i].r2 = 0x60;
                    g_battle_data->gt3[i].g0 = 0x18;
                    g_battle_data->gt3[i].g1 = 0x18;
                    g_battle_data->gt3[i].g2 = 0x18;
                    g_battle_data->gt3[i].b0 = 0x20;
                    g_battle_data->gt3[i].b1 = 0x20;
                    g_battle_data->gt3[i].b2 = 0x20;
                    g_battle_map_textured_triangle_positions[i].polygon_flags &= 0xfffe;
                }
            }
        }
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[1]; i++) {
            if (g_battle_map_textured_quad_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_quad_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_ABILITY_RANGE) {
                    g_battle_data->gt4[i].clut = (g_battle_data->gt4[i].clut & 0x803f) | 0x7840;
                    g_battle_data->gt4[i].r0 = 0x60;
                    g_battle_data->gt4[i].r1 = 0x60;
                    g_battle_data->gt4[i].r2 = 0x60;
                    g_battle_data->gt4[i].r3 = 0x60;
                    g_battle_data->gt4[i].g0 = 0x18;
                    g_battle_data->gt4[i].g1 = 0x18;
                    g_battle_data->gt4[i].g2 = 0x18;
                    g_battle_data->gt4[i].g3 = 0x18;
                    g_battle_data->gt4[i].b0 = 0x20;
                    g_battle_data->gt4[i].b1 = 0x20;
                    g_battle_data->gt4[i].b2 = 0x20;
                    g_battle_data->gt4[i].b3 = 0x20;
                    g_battle_map_textured_quad_positions[i].polygon_flags &= 0xfffe;
                }
            }
        }
        g_battle_target_tile_mark_modes[1] = BATTLE_TARGET_TINT_ABILITY_RANGE;
        other_state_a = g_battle_target_tile_mark_modes[0];
        other_state_b = g_battle_target_tile_mark_modes[2];
        if ((other_state_a | other_state_b) == 0 || (other_state_a == 5 && other_state_b == 8)) {
            battle_map_modify_palette(9, 8, 1, 0, 1, 10, 10, 10);
        }
        break;
    case BATTLE_TARGET_TINT_CLEAR_ABILITY_RANGE:
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[0]; i++) {
            if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_ABILITY_RANGE) {
                    g_battle_data->gt3[i].clut = (g_battle_data->gt3[i].clut & 0x803f) | 0x7800;
                    if (g_battle_map_textured_triangle_positions[i].polygon_flags & 0x8000) {
                        g_battle_map_textured_triangle_positions[i].polygon_flags |= 1;
                    } else {
                        (g_battle_data->gt3 + i)->r0 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt3 + i)->g0 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt3 + i)->b0 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt3 + i)->r1 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt3 + i)->g1 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt3 + i)->b1 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt3 + i)->r2 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt3 + i)->g2 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt3 + i)->b2 = g_battle_map_ambient_polygon_color[2];
                    }
                }
            }
        }
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[1]; i++) {
            if (g_battle_map_textured_quad_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_quad_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_ABILITY_RANGE) {
                    g_battle_data->gt4[i].clut = (g_battle_data->gt4[i].clut & 0x803f) | 0x7800;
                    if (g_battle_map_textured_quad_positions[i].polygon_flags & 0x8000) {
                        g_battle_map_textured_quad_positions[i].polygon_flags |= 1;
                    } else {
                        (g_battle_data->gt4 + i)->r0 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g0 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b0 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt4 + i)->r1 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g1 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b1 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt4 + i)->r2 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g2 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b2 = g_battle_map_ambient_polygon_color[2];
                        (g_battle_data->gt4 + i)->r3 = g_battle_map_ambient_polygon_color[0];
                        (g_battle_data->gt4 + i)->g3 = g_battle_map_ambient_polygon_color[1];
                        (g_battle_data->gt4 + i)->b3 = g_battle_map_ambient_polygon_color[2];
                    }
                }
            }
        }
        g_battle_target_tile_mark_modes[1] = BATTLE_TARGET_TINT_CLEAR_ABILITY_RANGE;
        g_battle_target_tile_color_buffer = g_battle_data;
        break;
    case BATTLE_TARGET_TINT_TARGETED:
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[0]; i++) {
            if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_TARGETED) {
                    g_battle_data->gt3[i].clut = (g_battle_data->gt3[i].clut & 0x803f) | 0x7840;
                    g_battle_data->gt3[i].r0 = 0x50;
                    g_battle_data->gt3[i].r1 = 0x50;
                    g_battle_data->gt3[i].r2 = 0x50;
                    g_battle_data->gt3[i].g0 = 0x60;
                    g_battle_data->gt3[i].g1 = 0x60;
                    g_battle_data->gt3[i].g2 = 0x60;
                    g_battle_data->gt3[i].b0 = 0x10;
                    g_battle_data->gt3[i].b1 = 0x10;
                    g_battle_data->gt3[i].b2 = 0x10;
                    g_battle_map_textured_triangle_positions[i].polygon_flags &= 0xfffe;
                }
            }
        }
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[1]; i++) {
            if (g_battle_map_textured_quad_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_quad_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                if (g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks & MAP_TILE_FLAG_TARGETED) {
                    g_battle_data->gt4[i].clut = (g_battle_data->gt4[i].clut & 0x803f) | 0x7840;
                    g_battle_data->gt4[i].r0 = 0x50;
                    g_battle_data->gt4[i].r1 = 0x50;
                    g_battle_data->gt4[i].r2 = 0x50;
                    g_battle_data->gt4[i].r3 = 0x50;
                    g_battle_data->gt4[i].g0 = 0x60;
                    g_battle_data->gt4[i].g1 = 0x60;
                    g_battle_data->gt4[i].g2 = 0x60;
                    g_battle_data->gt4[i].g3 = 0x60;
                    g_battle_data->gt4[i].b0 = 0x10;
                    g_battle_data->gt4[i].b1 = 0x10;
                    g_battle_data->gt4[i].b2 = 0x10;
                    g_battle_data->gt4[i].b3 = 0x10;
                    g_battle_map_textured_quad_positions[i].polygon_flags &= 0xfffe;
                }
            }
        }
        g_battle_target_tile_mark_modes[2] = BATTLE_TARGET_TINT_TARGETED;
        other_state_a = g_battle_target_tile_mark_modes[0];
        other_state_b = g_battle_target_tile_mark_modes[1];
        if ((other_state_a | other_state_b) == 0 || (other_state_a == 5 && other_state_b == 3)) {
            battle_map_modify_palette(9, 8, 1, 0, 1, 10, 10, 10);
        }
        break;
    case BATTLE_TARGET_TINT_CLEAR_TARGETED:
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[0]; i++) {
            if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_triangle_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_triangle_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                marks = g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks;
                if (marks & 0x80) {
                    if (!(marks & 0x40)) {
                        g_battle_data->gt3[i].clut = (g_battle_data->gt3[i].clut & 0x803f) | 0x7800;
                        if (g_battle_map_textured_triangle_positions[i].polygon_flags & 0x8000) {
                            g_battle_map_textured_triangle_positions[i].polygon_flags |= 1;
                        } else {
                            (g_battle_data->gt3 + i)->r0 = g_battle_map_ambient_polygon_color[0];
                            (g_battle_data->gt3 + i)->g0 = g_battle_map_ambient_polygon_color[1];
                            (g_battle_data->gt3 + i)->b0 = g_battle_map_ambient_polygon_color[2];
                            (g_battle_data->gt3 + i)->r1 = g_battle_map_ambient_polygon_color[0];
                            (g_battle_data->gt3 + i)->g1 = g_battle_map_ambient_polygon_color[1];
                            (g_battle_data->gt3 + i)->b1 = g_battle_map_ambient_polygon_color[2];
                            (g_battle_data->gt3 + i)->r2 = g_battle_map_ambient_polygon_color[0];
                            (g_battle_data->gt3 + i)->g2 = g_battle_map_ambient_polygon_color[1];
                            (g_battle_data->gt3 + i)->b2 = g_battle_map_ambient_polygon_color[2];
                        }
                    } else {
                        g_battle_data->gt3[i].r0 = 0x60;
                        g_battle_data->gt3[i].r1 = 0x60;
                        g_battle_data->gt3[i].r2 = 0x60;
                        g_battle_data->gt3[i].g0 = 0x18;
                        g_battle_data->gt3[i].g1 = 0x18;
                        g_battle_data->gt3[i].g2 = 0x18;
                        g_battle_data->gt3[i].b0 = 0x20;
                        g_battle_data->gt3[i].b1 = 0x20;
                        g_battle_data->gt3[i].b2 = 0x20;
                    }
                }
            }
        }
        for (i = 0; i < g_battle_map_mesh_parts[0].counts[1]; i++) {
            if (g_battle_map_textured_quad_positions[i].terrain_tile.packed != 0xfffe) {
                g_battle_target_color_tile_y
                    = (g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 1) & 0x7f;
                g_battle_target_color_tile_x = g_battle_map_textured_quad_positions[i].terrain_tile.packed >> 8;
                tile = (s16)g_battle_target_color_tile_y * g_battle_map_tile_width + (s16)g_battle_target_color_tile_x;
                if (g_battle_map_textured_quad_positions[i].terrain_tile.packed & 1) {
                    tile += 0x100;
                }
                marks = g_battle_map_tile_data_ptr[tile].ceiling_depth_and_marks;
                if (marks & 0x80) {
                    if (!(marks & 0x40)) {
                        g_battle_data->gt4[i].clut = (g_battle_data->gt4[i].clut & 0x803f) | 0x7800;
                        if (g_battle_map_textured_quad_positions[i].polygon_flags & 0x8000) {
                            g_battle_map_textured_quad_positions[i].polygon_flags |= 1;
                        } else {
                            (g_battle_data->gt4 + i)->r0 = g_battle_map_ambient_polygon_color[0];
                            (g_battle_data->gt4 + i)->g0 = g_battle_map_ambient_polygon_color[1];
                            (g_battle_data->gt4 + i)->b0 = g_battle_map_ambient_polygon_color[2];
                            (g_battle_data->gt4 + i)->r1 = g_battle_map_ambient_polygon_color[0];
                            (g_battle_data->gt4 + i)->g1 = g_battle_map_ambient_polygon_color[1];
                            (g_battle_data->gt4 + i)->b1 = g_battle_map_ambient_polygon_color[2];
                            (g_battle_data->gt4 + i)->r2 = g_battle_map_ambient_polygon_color[0];
                            (g_battle_data->gt4 + i)->g2 = g_battle_map_ambient_polygon_color[1];
                            (g_battle_data->gt4 + i)->b2 = g_battle_map_ambient_polygon_color[2];
                            (g_battle_data->gt4 + i)->r3 = g_battle_map_ambient_polygon_color[0];
                            (g_battle_data->gt4 + i)->g3 = g_battle_map_ambient_polygon_color[1];
                            (g_battle_data->gt4 + i)->b3 = g_battle_map_ambient_polygon_color[2];
                        }
                    } else {
                        g_battle_data->gt4[i].r0 = 0x60;
                        g_battle_data->gt4[i].r1 = 0x60;
                        g_battle_data->gt4[i].r2 = 0x60;
                        g_battle_data->gt4[i].r3 = 0x60;
                        g_battle_data->gt4[i].g0 = 0x18;
                        g_battle_data->gt4[i].g1 = 0x18;
                        g_battle_data->gt4[i].g2 = 0x18;
                        g_battle_data->gt4[i].g3 = 0x18;
                        g_battle_data->gt4[i].b0 = 0x20;
                        g_battle_data->gt4[i].b1 = 0x20;
                        g_battle_data->gt4[i].b2 = 0x20;
                        g_battle_data->gt4[i].b3 = 0x20;
                    }
                }
            }
        }
        g_battle_target_tile_mark_modes[2] = BATTLE_TARGET_TINT_CLEAR_TARGETED;
        g_battle_target_tile_color_buffer = g_battle_data;
        break;
    }
}
