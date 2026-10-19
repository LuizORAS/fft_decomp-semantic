#include "fft/main.h"

/* Same as main_unit_update_stats_statuses_and_equipment with both flags clear: stats are re-derived
 * and the status notifications run. Level growth, EQUIP and a broken or stolen item call it. */
void main_unit_refresh_stats_and_statuses(battle_stats_t* unit) {
    main_unit_update_stats_statuses_and_equipment(unit, 0, 0);
}
