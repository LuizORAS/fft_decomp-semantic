#include "fft/world.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Tutorial command END: the first call starts the fade out; once it has finished, free the
 * tutorial's files, unpack the player's game back from g_world_tutorial_saved_game_image, rebuild the
 * formation's unit names and records, clear the tutorial id and return 0. While the fade runs it falls
 * off its end (see QUIRKS.md). */
s32 world_tutorial_command_end(void) {
    if (g_world_tutorial_command_active != 0) {
        if (world_gfx_update_fade_out_tile() == 0) {
            g_world_tutorial_id = 0;
            main_heap_call_free(g_world_tutorial_saved_game_image.messages);
            main_heap_call_free(g_world_tutorial_saved_game_image.script);
            bcopy(&g_world_tutorial_saved_game_image.saved_game, g_world_load_work_buffer,
                sizeof(g_world_tutorial_saved_game_image.saved_game));
            world_card_load_globals_from_save_image(1);
            world_text_generate_formation_unit_name_string();
            main_heap_call_free(g_world_load_work_buffer);
            world_formation_build_record_list(
                0, (world_unit_t**)&g_world_formation_unit_pointers, WORLD_FORMATION_RECORD_LIST_ALL);
            return 0;
        }
    } else {
        world_gfx_start_increasing_fade();
        g_world_tutorial_command_active = 1;
    }
}
