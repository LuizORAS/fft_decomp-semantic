#include "fft/battle.h"
#include "psx/types.h"

/* Pack the names of the 21 battle units into the roster-unit-names text section,
 * one after another.
 *
 * A name is copied up to and including its first 0xfe or 0xff byte; a 16-byte
 * name without one gets a 0xfe appended, and an empty unit slot contributes a
 * lone 0xfe. */
void battle_text_build_unit_name_list(void) {
    u8* output;
    s32 unit_id;
    battle_stats_t* unit;
    s32 length;

    battle_text_init_menu_section_pointers();
    unit_id = 0;
    output = g_battle_text_section_pointers[TEXT_SECTION_ROSTER_UNIT_NAMES];
    do {
        unit = battle_unit_get_stats_from_battle_id(unit_id);
        if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
            *output++ = 0xfe;
        } else {
            length = 0;
            do {
                u8 value = unit->name[length];
                *output = value;
                if ((value & 0xfe) != 0xfe) {
                    output++;
                    if (length == 15) {
                        *output++ = 0xfe;
                    }
                } else {
                    output++;
                    break;
                }
                length++;
            } while (length < 16);
        }
        unit_id++;
    } while (unit_id < BATTLE_UNIT_SLOT_COUNT);
}
