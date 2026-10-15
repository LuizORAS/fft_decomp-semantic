#include "fft/world.h"
#include "psx/types.h"

/* Add delta to the war funds (EVENT_SCRIPT_VAR_WAR_FUNDS), clamped to 0..99,999,999, and
 * return the new amount: a purchase passes minus its price, a sale its price, and 0 reads the funds.
 * It is also menu value callback 0 of the gil windows. */
s32 world_shop_add_gil(s32 delta) {
    s32 gil;

    gil = world_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS) + delta;
    if (gil < 0) {
        gil = 0;
    }
    if (gil > 0x5F5E0FF) {
        gil = 0x5F5E0FF;
    }
    world_script_set_variable(EVENT_SCRIPT_VAR_WAR_FUNDS, gil);
    return gil;
}
