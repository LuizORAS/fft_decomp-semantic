#include "fft/battle_effect.h"
#include "fft/main.h"

/* Reports effect error 0x320 through main_system_report_error_2 (source 1,
 * effect code), an empty hook, so it does nothing in retail. Nothing on the
 * disc references it. */
void battle_effect_report_stub(void) {
    main_system_report_error_2(1, 0x320);
}
