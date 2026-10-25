#include "fft/world.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Start Formation tutorial `index` (0 only clears the tutorial id): load its message and script files
 * (world_tutorial_load_files) and bind them, then swap the game for the tutorial's. The current game is
 * serialized into g_world_tutorial_saved_game_image, and the tutorial save (FFTSAVE.DAT, 0x2000 bytes)
 * is loaded and unpacked into the globals, keeping the player's Max Equip at job change and sound
 * options, with unequippable items shown. world_tutorial_command_end swaps the game back. */
void world_tutorial_start(s32 index) {
    g_world_tutorial_id = index;
    if (index != 0) {
        world_tutorial_load_files(index, &g_world_tutorial_saved_game_image);
        g_world_tutorial_command_ptr = g_world_tutorial_saved_game_image.script;
        g_text_section_pointers[WORLD_TEXT_SECTION_TUTORIAL_MESSAGES] = g_world_tutorial_saved_game_image.messages;
        world_card_init_menu_state();
        world_card_build_save_image(-1);
        bcopy(g_world_load_work_buffer, &g_world_tutorial_saved_game_image.saved_game,
            sizeof(g_world_tutorial_saved_game_image.saved_game));
        main_file_load_to_address_checked(DISC_SECTOR_MENU_FFTSAVE_DAT, 0x2000, g_world_load_work_buffer);
        g_world_load_work_buffer->options.show_unequippable_items = GAME_OPTION_ON;
        g_world_load_work_buffer->options.max_equip_at_job_change = g_main_game_options.fields.max_equip_at_job_change;
        g_world_load_work_buffer->options.sound_mode = g_main_game_options.fields.sound_mode;
        world_card_load_globals_from_save_image(1);
        g_world_tutorial_command_active = 0;
        world_text_generate_formation_unit_name_string();
    }
}
