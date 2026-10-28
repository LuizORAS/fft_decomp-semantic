#include "fft/battle_effect.h"
#include "fft/main.h"

/* Reports effect error 0x320 through main_system_report_error_2 (source 1,
 * effect code), an empty hook, and returns 0. Nothing on the disc references
 * it. */
s32 battle_effect_report_stub_return_zero(void) {
    main_system_report_error_2(1, 0x320);
    return 0;
}
