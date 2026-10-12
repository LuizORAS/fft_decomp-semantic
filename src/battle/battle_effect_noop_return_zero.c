#include "fft/battle_effect.h"
#include "fft/main.h"

/* Effect-table no-op callback; the zero return selects the normal path. */
s32 battle_effect_noop_return_zero(void) {
    main_system_report_stub(1, 0x320);
    return 0;
}
