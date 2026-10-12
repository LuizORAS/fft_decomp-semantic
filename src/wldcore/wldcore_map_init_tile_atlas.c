#include "fft/wldcore.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Lay out the world map's tile grid and reset its camera, once per visit from
 * wldcore_init_subsystems. Each of the 13 x 17 grid corners takes the next six-word tile descriptor
 * (texture page x and y, texture u and v, width, height): the corners run from (-256, -192) by the
 * tile sizes, and a tile with no width or height is empty. A restart keeps the grid. The camera gets
 * scale ONE and no rotation or offset; then the map frame and root menu frame windows are appended
 * (records 0 and 1, the pools having just been zeroed; the root frame's index goes to
 * g_wldcore_menu_stack_types[0], where wldcore_gfx_toggle_captured_world_frame finds it) and the grid
 * is projected.
 *
 * Layout:
 *  - 0x8009f2d0 g_wldcore_map_projection_scale is a VECTOR;
 *    wldcore_map_project_and_cull_tiles reaches it as rotation - 0x10 off
 *    g_wldcore_map_projection_rotation_bytes.
 *  - g_wldcore_map_projection_rotation is the SVECTOR view of 0x8009f2e0
 *    (three `sh` stores at +0/+2/+4); the byte view keeps its own name.
 *  - The 24-byte tile source table is reached through the pointer cell
 *    g_wldcore_map_tile_descriptors. The target indexes it by word
 *    (base + k*4, k += 6), not by record, so it is an s32* with a word index.
 *
 * Shapes the bytes depend on:
 *  - `.map_x`/`.map_y` are written through one loop-invariant s32* to element
 *    0's `.map_x`, as pos[i * 6] / pos[i * 6 + 1]. That makes the address a
 *    strength_reduce giv, initialised at the row head with the target's
 *    lui/addiu/addu triple and carried with += 24. Two array-form stores each
 *    pay a full lui/addiu/addu/sw site; a source-level induction variable
 *    hoists the init into the outer preheader and is one instruction short.
 *  - The window block uses the same mixed spelling: `.x`/`.y` through one
 *    handle and `.anim_counter` off the record base, while
 *    `.sequence`/`.priority`/`.frame_index` stay array-form.
 *  - A separate index local per append call deletes two `move v1,v0` copies.
 *  - Each window-record group has its own pointer pair (wpos/rec and
 *    wpos2/rec2). One shared pair is long-lived and materialises both
 *    addresses into a2/a3 ahead of the first call; split pairs are short-lived
 *    temps, giving the target's per-group `addu v0,v1,s0` / `addu v0,v1,s1`
 *    while s0 (the `.x` column) and s1 (the record base, CSE'd as
 *    `addiu s1,s0,-0x18`) stay live across both groups.
 *  - `y` is initialised before `k` and `i`: the target emits `li t3,-192`
 *    first in the outer preheader. */
void wldcore_map_init_tile_atlas(void) {
    s32* descriptor;
    s32* wpos;
    s32* wpos2;
    wldcore_window_record_t* rec;
    wldcore_window_record_t* rec2;
    s32* pos;
    s32 map_window_index;
    s32 menu_window_index;
    s32 k;
    s32 i;
    s32 row;
    s32 col;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 texture_u;
    s32 texture_v;
    s32 tpage_x;
    s32 tpage_y;

    g_wldcore_map_projection_scale.vx = ONE;
    g_wldcore_map_projection_scale.vy = ONE;
    g_wldcore_map_projection_scale.vz = ONE;
    g_wldcore_map_projection_origin.vx = 0;
    g_wldcore_map_projection_origin.vy = 0;
    g_wldcore_map_projection_origin.vz = 0;
    g_wldcore_map_projection_rotation.vx = 0;
    g_wldcore_map_projection_rotation.vy = 0;
    g_wldcore_map_projection_rotation.vz = 0;

    descriptor = g_wldcore_map_tile_descriptors;
    if (!(g_main_system_flags & MAIN_SYSTEM_FLAG_RESTART)) {
        y = -192;
        k = 0;
        i = 0;
        pos = &g_wldcore_map_projected_tiles[0].map_x;
        for (row = 0; row < 13; row++) {
            x = -256;
            for (col = 0; col < 17; col++) {
                tpage_x = descriptor[k];
                tpage_y = descriptor[k + 1];
                texture_u = descriptor[k + 2];
                texture_v = descriptor[k + 3];
                width = descriptor[k + 4];
                height = descriptor[k + 5];
                pos[i * 6] = x;
                pos[i * 6 + 1] = y;
                if (width != 0 && height != 0) {
                    g_wldcore_map_projected_tiles[i].flags = GetTPage(1, 0, tpage_x, tpage_y) & 0xff;
                    g_wldcore_map_projected_tiles[i].texture = ((texture_u & 0xff) << 24) | ((texture_v & 0xff) << 16)
                        | ((width & 0xff) << 8) | (height & 0xff);
                } else {
                    g_wldcore_map_projected_tiles[i].flags = WLDCORE_MAP_TILE_EMPTY;
                }
                i++;
                x += width;
                k += 6;
            }
            y += height;
        }
    }

    map_window_index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_map_window_index = map_window_index;
    g_wldcore_window_records[map_window_index].sequence = WLDCORE_ANIM_SEQUENCE_MAP_FRAME;
    g_wldcore_window_records[map_window_index].priority = 13;
    wpos = &g_wldcore_window_records[map_window_index].position.x;
    rec = &g_wldcore_window_records[map_window_index];
    wpos[0] = 0;
    wpos[1] = 8;
    rec->anim_counter = 0;
    g_wldcore_window_records[map_window_index].frame_index = 0;

    menu_window_index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_types[0] = menu_window_index;
    g_wldcore_window_records[menu_window_index].sequence = WLDCORE_ANIM_SEQUENCE_ROOT_MENU_FRAME;
    g_wldcore_window_records[menu_window_index].priority = 13;
    wpos2 = &g_wldcore_window_records[menu_window_index].position.x;
    rec2 = &g_wldcore_window_records[menu_window_index];
    wpos2[0] = -4;
    wpos2[1] = 8;
    rec2->anim_counter = 0;
    g_wldcore_window_records[menu_window_index].frame_index = 0;

    wldcore_map_project_and_cull_tiles();
}
