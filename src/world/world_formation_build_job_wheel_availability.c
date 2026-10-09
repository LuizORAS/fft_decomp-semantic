#include "fft/world.h"
#include "psx/types.h"

/*
 * Build the selected unit's job-wheel choices and availability flags.
 *
 * Units away on propositions have no choices. Fewer than 2 choices bypass
 * the wheel; otherwise its opening animation starts at 20 frames.
 */
void world_formation_build_job_wheel_availability(void) {
    s32 i;
    s16 unit;
    u16 entry;
    s32 id;

    g_world_job_wheel_job_count
        = world_job_build_unit_list(g_world_formation_selected_unit_index, (s16*)g_world_job_wheel_jobs, 2);
    for (i = 0; i < g_world_job_wheel_job_count; i++) {
        unit = g_world_formation_selected_unit_index;
        entry = g_world_job_wheel_jobs[i];
        id = entry & 0x3FF;
        if (id == g_world_formation_unit_pointers[unit]->job_id || !(entry & 0x4000)) {
            g_world_job_wheel_mastered_jobs[i]
                = world_ability_find_unit_abilities(unit, id, 0xF, 0, WORLD_ABILITY_LIST_MODE_COUNT_UNLEARNED) == 0;
        } else {
            g_world_job_wheel_mastered_jobs[i] = 0;
        }
    }
    if (g_world_formation_unit_pointers[g_world_formation_selected_unit_index]->proposition_status != 0) {
        g_world_job_wheel_job_count = 0;
    }
    g_world_job_wheel_cursor_index = 0;
    world_formation_build_job_wheel_job_name();
    if (g_world_job_wheel_job_count < 2) {
        g_world_formation_current_menu = 3;
        return;
    }
    g_world_job_wheel_open_frame = 0x14;
    g_world_job_wheel_opening = 1;
}
