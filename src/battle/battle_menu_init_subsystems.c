#include "fft/battle.h"
#include "psx/types.h"

void battle_menu_init_subsystems(void) {
    battle_menu_init_subsystem_pointers();
    battle_menu_init_palette_and_text(0);
    battle_menu_init_stub();
    battle_menu_reset_subsystems();
    battle_map_reset_script_light_transform();
    battle_menu_init_panel_fade_tiles();
}
