#include "fft/menu.h"
#include "fft/open.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Per-frame handler of the New Game controller (handler 4).
 *
 * Until step 4 it waits for the overlay fade and runs Alazlam's prologue (open_prologue_update)
 * to its end. Step 4 builds Ramza's roster (loading WORLD if needed, on EVENT/OPEN.LZW), copies the displayed
 * frame into the frame image and the other buffer, runs the WORLD name entry screen over it, stores the
 * 16-byte name in party slot 0 and replaces itself with the birthday date controller.
 *
 * Step 5 unloads the music and opens the Music Test, but nothing sets step 5 (QUIRKS.md). Community sources
 * say the Japanese release reached it by naming Ramza "BGM kikitai" ("I want to hear the BGM") in kana. */
void open_title_step_new_game_start(open_controller_record_t* record) {
    open_title_new_game_state_t* state = &record->new_game;
    RECT rect;
    u8* name;
    party_unit_t* party;

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
        world_menu_init_with_open_text();

        /* Copy the displayed buffer into the frame image, then into the draw buffer. */
        if (g_main_gfx_active_buffer_index != 0) {
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

        if (g_main_gfx_active_buffer_index == 0) {
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
        name = world_name_run_entry_screen(text_find_entry(TEXT_ID_UNIT_NAME_SPECIAL_BASE), g_open_file_destination);
        party = main_party_get_unit(0);
        memcpy(party->name, name, sizeof(party->name));
        g_open_current_controller_index -= 1;
        open_birthday_push_date_controller();
        return;
    }

    if (g_open_gfx_overlay_fade.flags & OPEN_OVERLAY_FADE_ACTIVE) {
        return;
    }
    if (open_prologue_update() != 0) {
        return;
    }
    open_prologue_finished_hook();
    state->step = 4;
}
