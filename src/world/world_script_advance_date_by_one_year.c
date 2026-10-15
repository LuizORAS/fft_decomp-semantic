#include "fft/world.h"
#include "psx/types.h"

/* Request a date advance of 365 days through its script variable. */
void world_script_advance_date_by_one_year(void) {
    world_script_set_variable(EVENT_SCRIPT_VAR_DATE_ADVANCE, 0x16D);
}
