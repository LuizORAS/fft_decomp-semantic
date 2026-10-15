#include "fft/open.h"

/* Begin Alazlam's prologue, the narration that ends by asking for Ramza's name and birthday: start
 * MAIN_MUSIC_NEW_GAME with the master volume ramping up from 0, upload its two text pages
 * (open_file_load_prologue_text_pages), read OPNBK image 1 as the frame image, request the 12 pages of
 * g_open_prologue_entries with their text in ordering-table entries 3 and 4, and push the New Game controller
 * (handler 4) at step 0 as the screen is revealed. */
void open_prologue_start(void) {
    s32 controller;
    s32 stack_padding[4];

    main_sound_set_master_volume(0, 1);
    main_sound_set_current_music_track(MAIN_MUSIC_NEW_GAME);
    main_sound_set_master_volume(0x3fff, 5);
    open_file_load_prologue_text_pages();
    open_file_build_openbk_header(1, g_open_file_destination);
    open_file_wait_for_pending();
    open_prologue_request(12, 3, g_open_prologue_entries);
    open_gfx_start_overlay_fade_out(0x20);

    controller = g_open_current_controller_index;
    g_open_controller_records_next[controller].new_game.step = 0;
    g_open_controller_handler_indices[controller] = OPEN_CONTROLLER_NEW_GAME_START;
    g_open_current_controller_index = controller + 1;
}
