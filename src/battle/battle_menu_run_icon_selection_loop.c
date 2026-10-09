#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Menu icon list with a wrapping cursor, run inline by its caller's thread:
 * builds the text image once, alternates two sprite records and clears their
 * 0x78 state words when they hold 0 and 2. Exits the thread afterwards only
 * when the thread's second parameter is zero.
 *
 * BATTLE twin of world_menu_run_icon_selection_loop. */
void battle_menu_run_icon_selection_loop(void) {
    RECT rect;
    world_menu_icon_record_t records[2];
    s32 cursor;
    world_menu_entry_t* param;
    void* buffer;
    s32 i;
    world_menu_icon_record_t* record;

    param = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    record = &records[0];
    cursor = param->selected_index;
    if (cursor == -1) {
        cursor = 0;
    }
    battle_menu_build_window_sprites(&rect, param, record);
    battle_copy_bytes(&records[1], record, 0x7C);
    buffer = battle_menu_build_and_upload_window_frame_image(param->inner_width, param->inner_height, &rect, 1);
    g_menu_text_state.pen.stride = param->inner_width;
    battle_menu_set_text_origin(8, 9);
    g_battle_menu_text_palette_offset = 0;
    battle_menu_display_text_entry(param->text_id, buffer, &g_menu_text_state.pen);
    LoadImage(&rect, buffer);
    for (i = 0;; i++) {
        battle_thread_yield();
        if (i == 0) {
            battle_menu_free_memory(buffer);
        }
        record = &records[i & 1];
        param->selected_index = cursor;
        battle_menu_select_icon_cluts(record);
        battle_menu_update_icon_cursor_sprites(param, &record->base, i, cursor);
        if ((records[0]._unknown_78 == 0 && records[1]._unknown_78 == 2)
            || (records[0]._unknown_78 == 2 && records[1]._unknown_78 == 0)) {
            records[0]._unknown_78 = 0;
            records[1]._unknown_78 = 0;
        }
        if (battle_menu_should_close_thread(&g_battle_script_event_input) != 0) {
            break;
        }
        battle_menu_handle_action(param, cursor);
        battle_menu_step_wrapping_cursor_on_scroll_buttons(param, &cursor);
        battle_handle_menu_cancel_input(param);
        battle_menu_submit_icon_primitives(record);
    }
    battle_thread_yield();
    if (battle_thread_get_current_parameter_2() == 0) {
        battle_thread_exit_current();
    }
}
