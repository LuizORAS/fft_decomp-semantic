#include "fft/world.h"
#include "psx/types.h"

/* Same as world_menu_init_with_world_text for a caller that is not the main thread (WLDCORE's
 * entry, the Formation screen before an event): it leaves g_world_thread_current_id alone, loads the
 * common graphics with the unit image pages and also uploads the WLDFACE portraits. */
void world_menu_init_with_world_text_and_faces(void) {
    s32 i;
    s32 value;
    s32 base;
    s32* src;
    s32* dst;

    world_menu_init_subsystem_pointers();
    world_menu_load_common_graphics(1);
    world_gfx_load_wldface_to_frame_buffer();
    g_world_thread_call_target = (void (*)(void))main_file_load_to_address_checked;
    world_thread_call_on_main_stack(DISC_SECTOR_EVENT_WORLD_LZW, 0xE800, g_world_menu_text_file_buffer);
    world_menu_reset_subsystems();
    world_script_set_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE, 0);
    g_world_menu_thread_menu_data = g_world_menu_default_entries;
    i = 0;
    src = g_world_menu_text_file_buffer;
    base = (s32)src + 0x80;
    dst = (s32*)g_world_text_section_pointers;
    do {
        value = *src++;
        *dst++ = value + base;
        i++;
    } while (i < 0x20);
    g_world_text_section_pointers[TEXT_SECTION_LOCATION_NAMES] = g_world_location_names_resident;
    world_script_set_variable(EVENT_SCRIPT_VAR_RAMZA_MANDATORY_IN_SQUAD, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SQUAD_COUNT, 1);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_PRIMARY_SQUAD_ID, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SECONDARY_SQUAD_ID, 1);
}
