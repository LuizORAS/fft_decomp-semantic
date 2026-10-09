#include "fft/world.h"
#include "psx/types.h"

void world_shop_reopen_soldier_office_menu_step(void) {
    world_thread_set_parameters(NATIVE_THREAD_SLOT_MESSAGE, 0x19, 0xf821, 0);
    g_world_shop_menu_step = WORLD_SHOP_STEP_SOLDIER_OFFICE_MENU;
    g_world_shop_hire_menu_initialized = 0;
    world_shop_run_obtain_gil_menu();
    world_shop_run_soldier_office_fee_menu_script();
    world_menu_run_script_with_callback_suppressed();
}
