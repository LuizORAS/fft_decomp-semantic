#include "fft/open.h"
#include "psx/pad.h"

void open_menu_start_music_test_controller(void) {
    s32 i;
    s32 four;
    s32 controller;

    open_file_build_openbk_header(1, g_open_file_destination);
    open_file_wait_for_pending();

    for (i = 0; i < 96; i++) {
        g_open_music_test_list_entries[i] = i + 0xc000;
        g_open_music_test_list_entry_flags[i] = 0;
    }

    world_script_set_event_speed(1);
    g_open_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    /* The same 4 is stored twice, as the formation OT index and as OPEN_MUSIC_TEST_STEP_OPEN; the local
     * keeps it in a saved register across the fade-out call instead of being rematerialized. */
    four = 4;
    g_open_menu_formation_otag_index = four;
    g_open_system_runtime_flags |= OPEN_SYSTEM_RUNTIME_FLAG_WORLD_MENU_FRAME;
    open_gfx_start_overlay_fade_out(0x20);

    controller = g_open_current_controller_index;
    g_open_controller_records_next[controller].music_test.music_id = 0;
    g_open_controller_records_next[controller].music_test.delay = 0;
    g_open_controller_records_next[controller].music_test.music_slot = 0;
    g_open_controller_records_next[controller].music_test.step = four;
    g_open_controller_records_next[controller].music_test.blink_frames = 0;
    g_open_controller_handler_indices[controller] = OPEN_CONTROLLER_MUSIC_TEST;
    g_open_current_controller_index = controller + 1;
}
