#include "fft/main.h"
#include "fft/memory.h"

void main_unit_copy_job_growths_and_multipliers(unit_stats_t* unit) {
    main_util_copy_byte_data(
        g_main_job_data_pointer[unit->job_id].stat_growths, unit->stat_growths, sizeof(unit->stat_growths));
}
