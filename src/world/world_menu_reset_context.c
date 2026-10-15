#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Reset WORLD's menu context around a full-screen menu (the formation, a shop, the card screen,
 * name entry, the world map's screen requests): wait for the GPU, reset the menu subsystems, relocate
 * the menu text file's pointer table, rebuild the formation unit name strings, and point the menu at
 * its default entries and the resident location names with text color 0. */
void world_menu_reset_context(void) {
    DrawSync(0);
    world_menu_init_subsystem_pointers();
    world_menu_reset_subsystems();
    world_text_relocate_pointer_table(g_world_menu_text_file_buffer);
    world_text_generate_formation_unit_name_string();
    DrawSync(0);
    g_world_menu_thread_menu_data = g_world_menu_default_entries;
    g_world_text_section_pointers[TEXT_SECTION_LOCATION_NAMES] = g_world_location_names_resident;
    g_world_menu_text_color = 0;
}
