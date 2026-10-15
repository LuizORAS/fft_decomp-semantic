#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Menu icon strip thread: rebuilds the text image every seventh frame and
 * alternates between two sprite records until input arrives. */
void world_menu_icon_strip_thread(void) {
    RECT rect;
    world_menu_icon_record_t records[2];
    u32 input;
    world_menu_entry_t* param;
    void* buffer;
    s32 i;
    s32 fresh;
    world_menu_icon_record_t* record;

    input = 0;
    param = world_thread_get_current_parameter_1();
    record = &records[0];
    world_menu_build_icon_record(&rect, param, record);
    world_util_copy_bytes(&records[1], record, 0x7C);
    fresh = 1;
    for (i = 0;; i++) {
        if (i % 7 == 0) {
            fresh = 1;
            buffer = world_menu_build_and_upload_window_frame_image(
                param->inner_width, param->inner_height, &rect, MENU_WINDOW_FRAME_BUILD);
            world_menu_set_text_origin(8, 9);
            g_world_menu_text_state.pen.stride = param->inner_width;
            world_menu_display_text_entry(param->text_id, buffer, &g_world_menu_text_state.pen);
            LoadImage(&rect, buffer);
        }
        world_thread_yield();
        if (fresh == 1) {
            world_menu_free_memory(buffer);
            fresh = 0;
        }
        if (world_menu_should_close_thread(&input) != 0) {
            break;
        }
        record = &records[i & 1];
        world_menu_select_icon_cluts(&record->base);
        world_menu_submit_icon_primitives(&record->base);
    }
    world_thread_yield();
    world_thread_exit_current();
}
