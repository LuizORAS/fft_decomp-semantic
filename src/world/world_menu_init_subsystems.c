#include "fft/world.h"

/* WORLD twin of battle_menu_init_subsystems. */
void world_menu_init_subsystems(void) {
    world_menu_init_subsystem_pointers();
    world_menu_init_palette_and_text(0);
    world_menu_init_stub();
    world_menu_reset_subsystems();
    world_map_reset_script_light_transform();
    world_menu_init_panel_fade_tiles();
}
