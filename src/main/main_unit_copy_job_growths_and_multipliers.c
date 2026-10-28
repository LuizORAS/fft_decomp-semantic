#include "fft/main.h"
#include "fft/memory.h"

/* Copy the job's five stat growth and multiplier pairs into the unit (main_unit_copy_job_data copies
 * them too). */
void main_unit_copy_job_growths_and_multipliers(battle_stats_t* unit) {
    main_mem_copy_byte_data_source_first(
        g_main_job_data_pointer[unit->job_id].stat_growths, unit->stat_growths, sizeof(unit->stat_growths));
}
