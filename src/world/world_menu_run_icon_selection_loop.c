#include "fft/memory.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Menu icon list with a wrapping cursor, run inline by its caller's thread:
 * builds the text image once, alternates two sprite records and clears their
 * 32-bit state fields at 0x78 when they hold 0 and 2. Exits the thread afterwards only
 * when the thread's second parameter is zero. */
void world_menu_run_icon_selection_loop(void) {
    RECT rect;
    menu_icon_record_t records[2];
    s32 cursor;
    menu_entry_t* param;
    u8* buffer;
    s32 i;
    menu_icon_record_t* record;

    param = (menu_entry_t*)world_thread_get_current_parameter_1();
    record = &records[0];
    cursor = param->selected_index;
    if (cursor == -1) {
        cursor = 0;
    }
    world_menu_build_icon_record(&rect, param, record);
    mem_copy_bytes_nonempty(&records[1], record, sizeof(records[1]));
    buffer = world_menu_build_and_upload_window_frame_image(
        param->inner_width, param->inner_height, &rect, MENU_WINDOW_FRAME_BUILD);
    g_menu_text_state.pen.stride = param->inner_width;
    world_menu_set_text_origin(8, 9);
    g_menu_text_palette_offset = 0;
    world_menu_display_text_entry(param->text_id, buffer, &g_menu_text_state.pen);
    LoadImage(&rect, buffer);
    for (i = 0;; i++) {
        thread_yield();
        if (i == 0) {
            world_menu_free_memory(buffer);
        }
        record = &records[i & 1];
        param->selected_index = cursor;
        world_menu_select_icon_cluts(&record->base);
        world_menu_update_icon_cursor_sprites(param, &record->base, i, cursor);
        if ((records[0]._unknown_78 == 0 && records[1]._unknown_78 == 2)
            || (records[0]._unknown_78 == 2 && records[1]._unknown_78 == 0)) {
            records[0]._unknown_78 = 0;
            records[1]._unknown_78 = 0;
        }
        if (world_menu_check_thread_completion(&g_script_event_input) != 0) {
            break;
        }
        world_menu_handle_entry_confirm(param, cursor);
        world_menu_step_wrapping_cursor_on_scroll_buttons(param, &cursor);
        world_menu_cancel_thread_group(param);
        world_menu_submit_icon_primitives(&record->base);
    }
    thread_yield();
    if (world_thread_get_current_parameter_2() == 0) {
        thread_exit_current();
    }
}
