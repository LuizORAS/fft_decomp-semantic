#include "fft/main.h"

/* Same as main_unit_update_stats_statuses_and_equipment with skip_status_check set: stats are
 * re-derived without the Critical and status notifications (the formation screen's refresh). */
void main_unit_update_formation_stats(battle_stats_t* unit) {
    main_unit_update_stats_statuses_and_equipment(unit, 1, 0);
}
