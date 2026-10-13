#include "fft/battle_gfx.h"

/* battle_gfx_prepare_post_action_display for a misc id. An absent unit raises
 * pointer exception 0xc and returns 0. */
s32 battle_gfx_prepare_post_action_display_by_misc_id(s32 misc_id) {
    battle_unit_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id);
    if (unit != 0) {
        battle_gfx_prepare_post_action_display(unit);
        return 1;
    }
    main_system_handle_pointer_exception(0xc);
    return 0;
}
