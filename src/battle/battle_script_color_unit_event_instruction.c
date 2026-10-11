#include "fft/battle.h"
#include "psx/types.h"

/* ColorUnit: start a palette modulation on a unit or a unit group.
 *
 * Operands: unit id or group (halfword), blend mode, red, green, blue (s8 each)
 * and a time. A plain unit id colours that one unit; id 0 and the group ids from
 * EVENT_UNIT_GROUP_FIRST run all 21 misc ids through
 * battle_script_filter_unit_id_by_mode. Nothing happens when a plain unit id does
 * not resolve. */
void battle_script_color_unit_event_instruction(u8* ptr) {
    u16 unit_id;
    s32 out;
    s32 i;

    unit_id = battle_script_load_halfword(ptr);
    if (battle_unit_try_get_misc_data_by_unit_id(&unit_id, &out) != 0) {
        ptr += 2;
        i = 0;
        do {
            if (battle_script_filter_unit_id_by_mode(&unit_id, (u16*)&i, &out) != 0) {
                battle_gfx_start_misc_unit_palette_modulation(
                    ptr[0], ptr[4], unit_id, ((s8*)ptr)[1], ((s8*)ptr)[2], ((s8*)ptr)[3]);
                if (out == 0) {
                    return;
                }
            }
            i += 1;
        } while (i < EVENT_UNIT_SLOT_COUNT);
    }
}
