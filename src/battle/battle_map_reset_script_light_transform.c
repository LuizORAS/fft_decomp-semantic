#include "fft/battle.h"
#include "psx/types.h"

/* Put the map light transform of the event scripts back to the map's own light: no rotation, unit
 * scale, and the current light direction and colour matrices as the base battle_map_light_thread
 * interpolates from. */
void battle_map_reset_script_light_transform(void) {
    void* buffer;

    g_battle_script_light_angles.vx = 0;
    g_battle_script_light_angles.vy = 0;
    g_battle_script_light_angles.vz = 0;
    g_battle_script_light_scale.vx = ONE;
    g_battle_script_light_scale.vy = ONE;
    g_battle_script_light_scale.vz = ONE;
    buffer = battle_map_light_matrix_command(MAP_LIGHT_MATRIX_COMMAND_GET_LIGHT, buffer, buffer, 0);
    battle_util_copy_bytes(g_battle_script_light_base_direction_matrix, buffer, 0x20);
    battle_util_copy_bytes(g_battle_script_light_base_color_matrix,
        battle_map_light_matrix_command(MAP_LIGHT_MATRIX_COMMAND_GET_COLOR, buffer, buffer, 0), 0x20);
}
