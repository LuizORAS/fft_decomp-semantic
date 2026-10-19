#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"

/* Count the runtime records drawn with the crystal spritesheet (BATTLE_SPRITESHEET_ID_CRYSTAL);
 * battle_map_check_deep_dungeon_crystal_and_init_state uses it. */
s32 battle_unit_count_crystals(void) {
    battle_unit_t* unit;
    s32 count;

    count = 0;
    unit = g_battle_unit_list_head;
    while (unit != 0) {
        if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_CRYSTAL) {
            count++;
        }
        unit = unit->previous;
    }
    return count;
}
