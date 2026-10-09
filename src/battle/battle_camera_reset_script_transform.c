#include "fft/battle.h"
#include "psx/types.h"

void battle_camera_reset_script_transform(void) {
    void* buffer;

    g_battle_script_light_angles.vx = 0;
    g_battle_script_light_angles.vy = 0;
    g_battle_script_light_angles.vz = 0;
    g_battle_script_light_scale.vx = ONE;
    g_battle_script_light_scale.vy = ONE;
    g_battle_script_light_scale.vz = ONE;
    buffer = battle_map_light_matrix_command(MAP_LIGHT_MATRIX_COMMAND_GET_LIGHT, buffer, buffer, 0);
    battle_copy_bytes(g_battle_script_light_base_direction_matrix, buffer, 0x20);
    battle_copy_bytes(g_battle_script_light_base_color_matrix,
        battle_map_light_matrix_command(MAP_LIGHT_MATRIX_COMMAND_GET_COLOR, buffer, buffer, 0), 0x20);
}
