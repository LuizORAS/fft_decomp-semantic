#include "fft/event_attack.h"
#include "psx/types.h"

/* Return the deployed-unit coordinate slots to fill: the first six on the
 * first call, the slots from index 6 on every later call. */
void* attack_deploy_get_coordinate_slots(void) {
    void* coordinate_slots;

    if (g_attack_deploy_second_pass != 0) {
        coordinate_slots = &g_attack_deployed_unit_map_coordinates[6];
    } else {
        g_attack_deploy_second_pass = 1;
        coordinate_slots = g_attack_deployed_unit_map_coordinates;
    }

    return coordinate_slots;
}
