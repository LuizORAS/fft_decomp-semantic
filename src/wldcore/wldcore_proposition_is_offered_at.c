#include "fft/wldcore.h"
#include "psx/types.h"

/* Whether proposition `index` is offered at a location (`location` is the location + 1): its row's
 * location must match unless it is 0 (anywhere), and each condition its flags select must hold: the
 * shop level (EVENT_SCRIPT_VAR_SHOP_ITEM_AVAILABILITY) at least required_shop_level, today's zodiac
 * sign (wldcore_calendar_to_zodiac_date) equal to condition_value, or the proposition condition_value
 * completed. */
s32 wldcore_proposition_is_offered_at(s32 index, s32 location) {
    wldcore_proposition_fields_t entry;
    s32 month;
    s32 day;

    wldcore_proposition_unpack_row(&entry, index);
    if (entry.fields.location != 0) {
        if (location != entry.fields.location) {
            return 0;
        }
    }
    if (entry.fields.condition_flags & WLDCORE_PROPOSITION_CONDITION_SHOP_LEVEL) {
        if (world_script_get_variable(EVENT_SCRIPT_VAR_SHOP_ITEM_AVAILABILITY) < entry.fields.required_shop_level) {
            return 0;
        }
    }
    if (entry.fields.condition_flags & WLDCORE_PROPOSITION_CONDITION_MONTH) {
        month = world_script_get_variable(EVENT_SCRIPT_VAR_MONTH);
        day = world_script_get_variable(EVENT_SCRIPT_VAR_DAY);
        wldcore_calendar_to_zodiac_date(&month, &day);
        if (month != entry.fields.condition_value) {
            return 0;
        }
    }
    if (entry.fields.condition_flags & WLDCORE_PROPOSITION_CONDITION_STATUS) {
        if ((world_script_get_variable(entry.fields.condition_value + EVENT_SCRIPT_VAR_PROPOSITION_FIRST)
                & WLDCORE_PROPOSITION_VARIABLE_COMPLETED)
            == 0) {
            return 0;
        }
    }
    return 1;
}
