#include "fft/main.h"
#include "psx/types.h"

/* main_unit_update_stats_statuses_and_equipment in its initializing form: the
 * inflicted statuses are seeded from the current ones and their CTs cleared. */
void main_unit_init_stats_and_statuses(battle_stats_t* unit) {
    main_unit_update_stats_statuses_and_equipment(unit, 0, 1);
}
