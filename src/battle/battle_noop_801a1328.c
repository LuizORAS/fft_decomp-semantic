#include "fft/battle.h"

/* Call the empty main_system_report_error_2 with (1, 0x320) from a 0x100-byte frame it does not use.
 * Nothing on the disc calls it or holds its address. */
void battle_noop_801a1328(void) {
    s32 unused[0x40];
    main_system_report_error_2(1, 0x320);
}
