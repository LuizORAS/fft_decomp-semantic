#include "fft/main.h"
#include "psx/types.h"

void main_overlay_exec_battle_bin(void) {
    main_file_load_checked_to_address(DISC_SECTOR_BATTLE_BIN,
        g_main_heap_high_overlay_load_address - g_main_heap_low_overlay_load_address,
        g_main_heap_low_overlay_load_address);
    main_overlay_call_battle_entrypoint();
}
