#include "fft/world.h"
#include "psx/types.h"

/* Variant of world_menu_load_text_1bd8 that loads no graphics and reads the
 * 0x5800-byte text file at sector 0x1C18; all 0x20 section pointers come
 * from the file, relative to the end of its offset table. */
void world_menu_load_text_1c18(void) {
    s32 i;

    world_menu_init_subsystem_pointers();
    g_world_thread_call_target = (void (*)(void))main_file_load_checked_to_address;
    world_thread_call_on_main_stack(DISC_SECTOR_EVENT_OPEN_LZW, 0x5800, g_world_menu_text_file_buffer);
    world_menu_reset_subsystems();
    world_script_set_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE, 0);
    g_world_menu_thread_menu_data = g_world_menu_default_entries;
    for (i = 0; i < TEXT_SECTION_COUNT; i++) {
        g_text_section_pointers[i]
            = g_world_menu_text_file_buffer[i] + (u8*)&g_world_menu_text_file_buffer[TEXT_SECTION_COUNT];
    }
    world_script_set_variable(EVENT_SCRIPT_VAR_RAMZA_MANDATORY_IN_SQUAD, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SQUAD_COUNT, 1);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_PRIMARY_SQUAD_ID, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SECONDARY_SQUAD_ID, 1);
}
