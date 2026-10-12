#include "fft/battle_effect.h"
#include "fft/main.h"

/* Report an effect stub and return zero. */
s32 effect_report_stub_return_zero(void) {
    main_system_report_stub(1, 0x320);
    return 0;
}
