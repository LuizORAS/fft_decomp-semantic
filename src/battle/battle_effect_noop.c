#include "fft/battle.h"

/* Effect-table no-op callback used where no effect-side action is needed. */
void battle_effect_noop(void) {
    main_system_report_error_2(1, 0x320);
}
