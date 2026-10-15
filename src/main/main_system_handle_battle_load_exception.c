#include "fft/main.h"

/* Report error category 3 (battle load) with error_code. */
void main_system_handle_battle_load_exception(s32 error_code) {
    main_system_report_error(3, error_code);
}
