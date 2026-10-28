#include "fft/battle.h"

/* Call the empty main_system_report_error_2 with (1, 0x50). Nothing on the disc calls it or holds its address. */
void battle_noop_801a866c(void) {
    main_system_report_error_2(1, 0x50);
}
