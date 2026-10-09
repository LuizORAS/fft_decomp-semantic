#include "fft/world.h"

s32 world_job_get_selected_unit_job_points_entry(s32 unused_row) {
    return g_world_formation_unit_pointers[g_world_formation_selected_unit_index]
        ->job_points[g_world_learn_selected_job_index];
}
