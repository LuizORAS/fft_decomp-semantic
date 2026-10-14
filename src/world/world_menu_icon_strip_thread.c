#include "fft/memory.h"
#include "fft/menu.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Menu icon strip thread: rebuilds the text image every seventh frame and
 * alternates between two sprite records until input arrives. */
void world_menu_icon_strip_thread(void) {
    RECT rect;
    menu_icon_record_t records[2];
    u32 input;
    menu_entry_t* param;
    u8* buffer;
    s32 i;
    s32 fresh;
    menu_icon_record_t* record;

    input = 0;
    param = (menu_entry_t*)world_thread_get_current_parameter_1();
    record = &records[0];
    world_menu_build_icon_record(&rect, param, record);
    mem_copy_bytes_nonempty(&records[1], record, sizeof(records[1]));
    fresh = 1;
    for (i = 0;; i++) {
        if (i % 7 == 0) {
            fresh = 1;
            buffer = world_menu_build_and_upload_window_frame_image(
                param->inner_width, param->inner_height, &rect, MENU_WINDOW_FRAME_BUILD);
            world_menu_set_text_origin(8, 9);
            g_menu_text_state.pen.stride = param->inner_width;
            world_menu_display_text_entry(param->text_id, buffer, &g_menu_text_state.pen);
            LoadImage(&rect, buffer);
        }
        thread_yield();
        if (fresh == 1) {
            world_menu_free_memory(buffer);
            fresh = 0;
        }
        if (world_menu_check_thread_completion(&input) != 0) {
            break;
        }
        record = &records[i & 1];
        world_menu_select_icon_cluts(&record->base);
        world_menu_submit_icon_primitives(&record->base);
    }
    thread_yield();
    thread_exit_current();
}
