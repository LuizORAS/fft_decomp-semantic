#include "fft/battle_script.h"

/* CallFunction 2 and 7: stage a 365-day advance in the date-advance variable. The
 * calendar itself moves when that variable is consumed. */
void script_advance_date_by_one_year(void) {
    script_set_variable(EVENT_SCRIPT_VAR_DATE_ADVANCE, 365);
}
