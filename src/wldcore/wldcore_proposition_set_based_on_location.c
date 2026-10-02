#include "fft/wldcore.h"
#include "psx/types.h"

/* Whether proposition `index` is offered at `location`: the row's location
 * (0 for any), then each condition its flags select. The status condition
 * tests bit 2 of script variable 0x360 + condition_value. */
s32 wldcore_proposition_set_based_on_location(s32 index, s32 location) {
    wldcore_proposition_fields_t entry;
    s32 month;
    s32 day;

    wldcore_unpack_proposition_row(&entry, index);
    if (entry.fields.location != 0) {
        if (location != entry.fields.location) {
            return 0;
        }
    }
    if (entry.fields.condition_flags & WLDCORE_PROPOSITION_CONDITION_SCRIPT_VAR_6F) {
        if (world_script_get_variable(EVENT_SCRIPT_VAR_SHOP_ITEM_AVAILABILITY) < entry.fields.required_script_var_6f) {
            return 0;
        }
    }
    if (entry.fields.condition_flags & WLDCORE_PROPOSITION_CONDITION_MONTH) {
        month = world_script_get_variable(EVENT_SCRIPT_VAR_MONTH);
        day = world_script_get_variable(EVENT_SCRIPT_VAR_DAY);
        wldcore_convert_date_to_zodiac_date(&month, &day);
        if (month != entry.fields.condition_value) {
            return 0;
        }
    }
    if (entry.fields.condition_flags & WLDCORE_PROPOSITION_CONDITION_STATUS) {
        if ((world_script_get_variable(entry.fields.condition_value + 0x360) & 4) == 0) {
            return 0;
        }
    }
    return 1;
}
