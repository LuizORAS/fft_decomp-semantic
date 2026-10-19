#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"

/* ResumeUnitPaletteUpdates (EVENT_OPCODE_RESUME_UNIT_PALETTE_UPDATES, enable 0) and
 * SuppressUnitPaletteUpdates (EVENT_OPCODE_SUPPRESS_UNIT_PALETTE_UPDATES, enable 1): clear or set
 * palette-update suppression on the event unit, or on every unit its group mode selects.
 *
 * The target stores a0 directly as a 16-bit value. The interpreter passes a zero-extended 32-bit value; signed
 * argument normalization is not part of this ABI. */
void battle_script_set_units_palette_update_suppression(s32 event_unit_id, s32 enable) {
    u16 unit_id;
    s32 mode;
    s32 i;

    unit_id = event_unit_id;
    if (script_prepare_unit_filter(&unit_id, &mode) != 0) {
        for (i = 0; i < EVENT_UNIT_SLOT_COUNT; i++) {
            if (script_filter_unit_id_by_mode(&unit_id, (u16*)&i, &mode) != 0) {
                if (enable == 0) {
                    battle_unit_clear_palette_update_suppression(unit_id);
                } else {
                    battle_unit_set_palette_update_suppression(unit_id);
                }
                if (mode == 0) {
                    return;
                }
            }
        }
    }
}
