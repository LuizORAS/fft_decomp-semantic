#include "fft/event_attack.h"
#include "psx/types.h"

/* Return 1 when roster unit `roster_id` stands on one of the 5x5 deployment tiles
 * (g_attack_deploy_roster_id_by_tile), 0 otherwise; the same code as attack_deploy_is_roster_unit_deployed. */
s32 attack_deploy_has_roster_id_on_tiles(s32 roster_id) {
    s32 row;
    s32 column;

    for (row = 0; row < 5; row++) {
        for (column = 0; column < 5; column++) {
            if (g_attack_deploy_roster_id_by_tile[row][column] == roster_id) {
                return 1;
            }
        }
    }
    return 0;
}
