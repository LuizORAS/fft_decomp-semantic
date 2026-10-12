#include "fft/wldcore.h"
#include "psx/libgs.h"
#include "psx/pad.h"

/* Push a location's menu level (WLDCORE_MENU_LEVEL_LOCATION_MENU) with the entries its caller stored
 * in the next level record: the town services, the Deep Dungeon floors or a red location's event
 * choices. It shows the location's name label, and its picture in a border when it has one, over a
 * list panel sized to the widest entry with at most four rows visible; the cursor keeps its saved
 * row.
 *
 * The identical `location += WLDCORE_ANIM_SEQUENCE_LOCATION_NAME_FIRST` arms (the name label's
 * sequence) split the block for the scheduler (jump2 cross-jumps them back together), keeping the add
 * after the picture call so reorg puts it in the wait call's delay slot. The border window's
 * position is stored through a stride-36 view based at &records->x, as the target forms
 * (records + 0x18) + index * 36. The unused array reproduces the 0xb8 frame. */
void wldcore_menu_push_location_menu_level(s32 location) {
    GsIMAGE image;
    wldcore_window_render_bounds16_t bounds;
    wldcore_point32_t origin;
    wldcore_point32_t dimensions;
    s16 text_width;
    s16 text_rows;
    s32 index;
    s32 count;
    s32 depth;
    s16 widest;
    s32 value;
    s32 kind;
    u32 flags;
    s32 i;
    wldcore_window_entry_36_rgb_t* color;
    wldcore_window_record_t* records;
    wldcore_location_list_level_t* level;
    wldcore_point32_t* base;
    wldcore_window_xy_t* position;

    g_wldcore_map_projection_state.marker.sub_kind
        = wldcore_gfx_load_location_picture(g_wldcore_map_projection_state.marker.kind);
    if (g_wldcore_map_projection_state.marker.sub_kind != 0) {
        location += WLDCORE_ANIM_SEQUENCE_LOCATION_NAME_FIRST;
    } else {
        location += WLDCORE_ANIM_SEQUENCE_LOCATION_NAME_FIRST;
    }
    wldcore_wait_for_file_load();
    flags = g_main_system_flags | MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
    level = &g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list;
    level->extra_render = -1;
    level->label_window = -1;
    level->frame_render = -1;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.cursor_window = -1;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.mode = 0;
    g_main_system_flags = flags;

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.cursor_window = index;
    records = g_wldcore_window_records;
    color = (wldcore_window_entry_36_rgb_t*)&records->color;
    g_wldcore_window_records[index].priority = 0xB;
    g_wldcore_window_records[index].sequence = location;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
    color[index].red = 0;
    color[index].green = 0;
    color[index].blue = 0;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.mode = 8;

    if (g_wldcore_map_projection_state.marker.sub_kind != 0) {
        GsGetTimInfo(g_wldcore_location_frame_tim + 1, &image);
        LoadTPage(image.pixel, 0, 0, image.px, image.py, image.pw * 4, image.ph);
        LoadClut(image.clut, image.cx, image.cy);
        GsGetTimInfo(g_wldcore_picture_buffer + 1, &image);
        LoadTPage(image.pixel, 0, 0, image.px, image.py, image.pw * 4, image.ph);
        LoadClut(image.clut, image.cx, image.cy);

        index = wldcore_window_append_render_record_and_reset_color(
            g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.frame_render = index;
        bounds.position.x = image.px;
        bounds.position.y = image.py;
        bounds.dimensions.x = image.pw;
        bounds.dimensions.y = image.ph;
        origin.x = image.cx;
        origin.y = image.cy;
        wldcore_window_init_vram_render_record(index, bounds, origin, WLDCORE_DISPLAY_OBJECT_8BIT_CLUT);
        base = &g_wldcore_window_render_records[index].base;
        g_wldcore_window_render_records[index].priority = 0xB;
        base->x = -0x40;
        base->y = -0x4C;
        g_wldcore_window_render_records[index].flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;

        index = wldcore_window_append_record_and_reset_color(
            g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.label_window = index;
        g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_LOCATION_PICTURE_BORDER;
        g_wldcore_window_records[index].priority = 0xB;
        records[index].anim_counter = 0;
        g_wldcore_window_records[index].frame_index = 0;
        position = (wldcore_window_xy_t*)&records->position.x;
        position[index].x = -8;
        position[index].y = 0x10;
        color[index].red = 0;
        color[index].green = 0;
        color[index].blue = 0;
    }

    widest = 0;
    count = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.entry_count;
    for (i = 0; i < count; i++) {
        world_text_measure_pixels(&text_width, &text_rows,
            world_text_find_entry(
                g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.entries[i]));
        if (widest < text_width) {
            widest = text_width;
        }
    }
    widest += 8;

    for (i = 0; i < count; i++) {
        value = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.entries[i];
        g_wldcore_list_entry_values[i] = value;
        g_wldcore_list_row_flags[i] = 0;
    }

    i = 4;
    if (count < 4) {
        i = count;
    }
    dimensions.x = widest;
    dimensions.y = i;
    origin.x = 0x80 - (((dimensions.x + 0x18) & 0xFFFC) >> 1);
    if (g_wldcore_map_projection_state.marker.sub_kind != 0) {
        origin.y = 0x80;
    } else {
        origin.y = 0x70 - i * 8;
    }
    wldcore_list_clamp_cursor_state(WLDCORE_CURSOR_SLOT_LOCATION_MENU, count, i);
    wldcore_window_init_panel_render_state(0xC,
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_list.entry_count, &dimensions, &origin);
    g_wldcore_window_panel_render_state.vram_x = 0x240;
    g_wldcore_window_panel_render_state.header_id = 0;
    g_wldcore_window_panel_render_state.vram_y = 0x100;

    if (g_wldcore_map_projection_state.marker.sub_kind != 0) {
        s32 unused[6]; /* frame padding: the target frame is 0xb8 */

        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                     .location_list.cursor_window]
            .position.x = -4;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                     .location_list.cursor_window]
            .position.y = 0;
    } else {
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                     .location_list.cursor_window]
            .position.x = -4;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                     .location_list.cursor_window]
            .position.y = g_wldcore_window_render_records[index].base.y - 8;
    }

    depth = g_wldcore_menu_stack_depth;
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    g_wldcore_menu_stack_records_next[depth].location_list.delay = 8;
    g_main_system_flags |= MAIN_SYSTEM_FLAG_FREEZE_ANIMATION;
    g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_LOCATION_MENU;
    g_wldcore_menu_stack_depth = depth + 1;
}
