#include "fft/world.h"

/* JP the selected unit has in the Learn Abilities menu's `index`th unlocked job. */
s32 world_menu_get_learn_job_jp(s32 index) {
    s32 job_index;

    job_index = world_job_get_generic_index(g_world_menu_learn_unlocked_jobs[index]);
    return g_world_formation_unit_pointers[g_world_formation_selected_unit_index]->job_points[job_index];
}
