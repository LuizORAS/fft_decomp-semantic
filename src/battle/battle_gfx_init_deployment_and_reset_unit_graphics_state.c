#include "fft/battle.h"
#include "fft/event_attack.h"
#include "psx/types.h"

void battle_gfx_init_deployment_and_reset_unit_graphics_state(void) {
    g_main_gfx_unit_graphics_load_queue.count = 0;
    if (battle_script_get_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE) == 0) {
        if (g_battle_deployment_skipped != 0) {
            battle_unit_build_deployed_units_data((battle_deployed_coords_t*)g_main_deployed_unit_map_coordinates, 0);
            g_battle_gfx_unit_graphics_load_state[0] = 0;
            g_battle_gfx_unit_graphics_load_state[1] = 0;
            g_battle_gfx_unit_graphics_load_state[2] = 0;
            g_battle_gfx_unit_graphics_load_state[3] = 0;
            return;
        }
    } else {
        battle_unit_init_deployed_units_data_for_debug_red_team(attack_deploy_get_coordinate_slots(), 0);
    }
    battle_unit_build_deployed_units_data(attack_deploy_get_coordinate_slots(), 0);
    g_battle_gfx_unit_graphics_load_state[0] = 0;
    g_battle_gfx_unit_graphics_load_state[1] = 0;
    g_battle_gfx_unit_graphics_load_state[2] = 0;
    g_battle_gfx_unit_graphics_load_state[3] = 0;
}
