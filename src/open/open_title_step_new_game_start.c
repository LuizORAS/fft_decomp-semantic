#include "fft/open.h"
#include "psx/libgpu.h"
#include "psx/types.h"

typedef struct open_title_party_name_block {
    /* 0x00 */ char name[16];
} open_title_party_name_block_t;

/* Per-frame handler of the new-game controller (handler 4).
 *
 * Until step 4 it waits for the overlay fade and runs the prologue
 * (open_gfx_update_transition_sequence) to its end. Step 4 builds the new-game
 * party (loading WORLD if needed), copies the displayed frame into the frame
 * image and the other buffer, runs the WORLD name-entry screen, stores the
 * 16-byte name in party slot 0 and replaces itself with the birthday date
 * controller.
 *
 * Step 5 unloads the music and starts the music test controller. Nothing in
 * OPEN.BIN stores 5 in step. */
void open_title_step_new_game_start(open_title_new_game_state_t* state) {
    RECT rect;
    open_title_party_name_block_t* name;
    party_data_t* party;

    if (state->step == 5) {
        if (g_open_gfx_overlay_fade.flags & OPEN_OVERLAY_FADE_ACTIVE) {
            return;
        }
        main_sound_unload_current_scenario_music();
        g_open_current_controller_index -= 1;
        open_menu_start_music_test_controller();
        return;
    }

    if (state->step == 4) {
        open_title_init_new_game_party(0, 0);
        world_menu_load_text_1c18();

        /* Copy the displayed buffer into the frame image, then into the draw buffer. */
        if (g_active_graphics_buffer_index != 0) {
            rect.x = 0;
            rect.y = 0;
            rect.w = 0x100;
            rect.h = 0xF0;
        } else {
            rect.x = 0;
            rect.y = 0xF0;
            rect.w = 0x100;
            rect.h = 0xF0;
        }
        StoreImage(&rect, g_open_file_destination);
        DrawSync(0);

        if (g_active_graphics_buffer_index == 0) {
            rect.x = 0;
            rect.y = 0;
            rect.w = 0x100;
            rect.h = 0xF0;
        } else {
            rect.x = 0;
            rect.y = 0xF0;
            rect.w = 0x100;
            rect.h = 0xF0;
        }
        LoadImage(&rect, g_open_file_destination);
        DrawSync(0);

        g_open_file_current_openbk_image_id = -1;
        name = (open_title_party_name_block_t*)world_name_run_entry_screen(
            world_text_find_entry(TEXT_ID_UNIT_NAME_SPECIAL_BASE), g_open_file_destination);
        party = main_party_get_data_pointer(0);
        *(open_title_party_name_block_t*)party->name = *name;
        g_open_current_controller_index -= 1;
        open_birthday_push_date_controller();
        return;
    }

    if (g_open_gfx_overlay_fade.flags & OPEN_OVERLAY_FADE_ACTIVE) {
        return;
    }
    if (open_gfx_update_transition_sequence() != 0) {
        return;
    }
    open_gfx_transition_finished_stub();
    state->step = 4;
}
