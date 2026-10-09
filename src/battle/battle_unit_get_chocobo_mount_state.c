#include "fft/battle.h"
#include "psx/types.h"

s32 battle_unit_get_chocobo_mount_state(battle_stats_t* unit) {
    if (unit->job_id < JOB_ID_CHOCOBO || unit->job_id > JOB_ID_RED_CHOCOBO)
        return BATTLE_CHOCOBO_MOUNT_NONE;
    if (main_unit_has_status_in_set(unit, MAIN_STATUS_CHECK_SET_UNMOUNTABLE) != 0)
        return BATTLE_CHOCOBO_MOUNT_NONE;
    if (unit->mount_info != 0)
        return BATTLE_CHOCOBO_MOUNT_PAIRED;
    return BATTLE_CHOCOBO_MOUNT_FREE;
}
