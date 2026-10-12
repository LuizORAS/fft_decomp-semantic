#include "fft/battle.h"
#include "fft/battle_map.h"
#include "fft/battle_script.h"

/* Put the map light transform of the event scripts back to the map's own light: no rotation, unit
 * scale, and the current light direction and colour matrices as the base map_light_thread
 * interpolates from. */
void map_reset_script_light_transform(void) {
    MATRIX* buffer;

    setVector(&g_script_light_angles, 0, 0, 0);
    setVector(&g_script_light_scale, ONE, ONE, ONE);
    buffer = map_light_matrix_command(MAP_LIGHT_MATRIX_COMMAND_GET_LIGHT, buffer, buffer, 0);
    battle_copy_bytes(&g_script_light_base_direction_matrix, buffer, 0x20);
    battle_copy_bytes(&g_script_light_base_color_matrix,
        map_light_matrix_command(MAP_LIGHT_MATRIX_COMMAND_GET_COLOR, buffer, buffer, 0), 0x20);
}
