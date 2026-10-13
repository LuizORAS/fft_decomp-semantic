#include "fft/memory.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Timed menu icon thread: shows one text entry with alternating sprite
 * records until input arrives or the g_world_text_message_duration_frames frame budget (scaled by the
 * event speed) runs out. Bit 8 of the budget suppresses cancel handling
 * until the budget expires. The thread-id check reads an unset local in the
 * original. */
void world_menu_icon_timed_thread(void) {
    RECT rect;
    menu_icon_record_t records[2];
    menu_entry_t* param;
    u8* buffer;
    s32 i;
    s32 hold;
    menu_icon_record_t* record;
    s16 width;
    s16 height;
    s32 pad;
    s32 unset;

    hold = 0;
    if (g_world_text_message_duration_frames >= 0x100) {
        hold = 1;
        g_world_text_message_duration_frames &= 0xFF;
    }
    i = 0;
    param = world_thread_get_current_parameter_1();
    world_menu_size_entry_to_text(param, &width, &height, &pad, 1);
    world_menu_build_icon_record(&rect, param, &records[0]);
    mem_copy_bytes_nonempty(&records[1], &records[0], sizeof(records[1]));
    buffer = world_menu_build_and_upload_window_frame_image(width, height, &rect, MENU_WINDOW_FRAME_BUILD);
    g_world_menu_text_state.pen.stride = width;
    world_menu_set_text_origin(pad + 8, 9);
    world_menu_display_text_entry(param->text_id, buffer, &g_world_menu_text_state.pen);
    LoadImage(&rect, buffer);
    for (;; i++) {
        thread_yield();
        if (i == 0) {
            world_menu_free_memory(buffer);
        }
        if (g_world_text_message_duration_frames < i * g_script_event_speed) {
            hold = 0;
            g_script_event_input = PSX_PAD_CIRCLE;
            g_world_menu_sound_muted = 1;
        }
        if (world_menu_check_thread_completion(&g_script_event_input) != 0) {
            break;
        }
        if (unset >= 0) {
            g_thread_current_id--;
            world_menu_select_icon_cluts(&records[i & 1].base);
            g_thread_current_id++;
        } else {
            world_menu_select_icon_cluts(&records[i & 1].base);
        }
        record = &records[i & 1];
        world_menu_update_icon_cursor_sprites(param, &record->base, i, -1);
        if (hold == 0) {
            world_menu_handle_entry_confirm(param, 0);
            world_menu_cancel_thread_group(param);
        }
        world_menu_submit_icon_primitives(&record->base);
    }
    g_world_menu_panel_fade_intensity = 0;
    g_world_menu_sound_muted = 0;
    thread_yield();
    if (world_thread_get_current_parameter_2() == 0) {
        thread_exit_current();
    }
}
