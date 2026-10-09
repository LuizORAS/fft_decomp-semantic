#include "fft/event_attack.h"
#include "psx/types.h"

void attack_deploy_mark_other_squad_units_undeployable(void) {
    s32 squad_id;
    s32 i;
    s32 unit_id;

    battle_copy_bytes(g_attack_deploy_roster_unit_deployable, g_attack_deploy_unit_fieldable, 20);
    for (squad_id = 0; squad_id < 4; squad_id++) {
        if (squad_id != g_attack_deploy_current_squad) {
            for (i = 0; i < 25; i++) {
                unit_id = g_attack_deploy_units_by_squad[squad_id][i];
                if (unit_id != 0xff) {
                    g_attack_deploy_roster_unit_deployable[unit_id] = 0;
                }
            }
        }
    }
}
