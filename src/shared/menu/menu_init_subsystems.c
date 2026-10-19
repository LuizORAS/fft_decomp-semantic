#include "fft/battle_map.h"
#include "fft/battle_menu.h"
#include "fft/menu.h"

/* Set up the battle menus: the subsystem pointers, the menu palette and text sections, the window
 * buffers, VRAM grid and buffer arena (battle_menu_reset_subsystems), the script light transform and
 * the panel fade tiles. WORLD compiles this text too (world_menu_init_subsystems). */
void menu_init_subsystems(void) {
    battle_menu_init_subsystem_pointers();
    battle_menu_init_palette_and_text(0);
    battle_menu_init_stub();
    battle_menu_reset_subsystems();
    map_reset_script_light_transform();
    menu_init_panel_fade_tiles();
}
