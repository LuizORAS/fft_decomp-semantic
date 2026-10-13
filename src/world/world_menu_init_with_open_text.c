#include "fft/world.h"
#include "psx/types.h"

/* Same as world_menu_init_with_world_text for the title screen's New Game
 * (open_title_step_new_game_start): no graphics, and the text file is EVENT/OPEN.LZW (0x5800 bytes),
 * all 0x20 section pointers taken from the file, relative to the end of its offset table. */
void world_menu_init_with_open_text(void) {
    s32 i;

    world_menu_init_subsystem_pointers();
    g_world_thread_call_target = (void (*)(void))main_file_load_to_address_checked;
    world_thread_call_on_main_stack(DISC_SECTOR_EVENT_OPEN_LZW, 0x5800, g_world_menu_text_file_buffer);
    world_menu_reset_subsystems();
    world_script_set_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE, 0);
    g_world_menu_thread_menu_data = g_world_menu_default_entries;
    for (i = 0; i < 0x20; i++) {
        g_world_text_section_pointers[i]
            = (u8*)(g_world_menu_text_file_buffer[i] + (s32)&g_world_menu_text_file_buffer[0x20]);
    }
    world_script_set_variable(EVENT_SCRIPT_VAR_RAMZA_MANDATORY_IN_SQUAD, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SQUAD_COUNT, 1);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_PRIMARY_SQUAD_ID, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SECONDARY_SQUAD_ID, 1);
}
