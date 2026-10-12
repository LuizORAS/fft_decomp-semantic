#include "fft/battle.h"
#include "psx/types.h"

/* CallFunction ADVANCE_YEAR and ADVANCE_YEAR_ALTERNATE: stage a 365-day advance in the date-advance
 * variable (EVENT_SCRIPT_VAR_DATE_ADVANCE). The calendar itself moves when WLDCORE consumes it. */
void battle_script_advance_date_by_one_year(void) {
    battle_script_set_variable(EVENT_SCRIPT_VAR_DATE_ADVANCE, 365);
}
