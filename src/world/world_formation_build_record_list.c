#include "fft/main.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Rebuilds the compacted WORLD formation records from the 20 roster slots.
 *
 * Mode 1 drops units with a proposition in progress and the egg-unit flag, a
 * monster skillset, or a special-monster job; mode 2 excludes eggs
 * and human units. A nonzero item id keeps only
 * units with that item equipped. The list argument is unused here; the sorter
 * at 0x80121c60 fills g_world_unit_pointers.
 */
s16 world_formation_build_record_list(s32 item_id, world_unit_t** unused_list, s32 mode) {
    unit_stats_t unit;
    s32 slot;
    s32 count;
    s32 id;
    s32 found;
    s32 i;

    slot = 0;
    count = 0;
    id = item_id & WORLD_ITEM_ID_MASK;
    for (; slot < PARTY_ROSTER_SLOT_COUNT; slot++) {
        if (main_unit_init_job_data(&unit, slot, 1) != 0) {
            continue;
        }
        world_formation_build_unit_record(&unit, &g_world_unit_records[count], main_party_get_unit(slot));
        if (mode == WORLD_FORMATION_RECORD_LIST_AVAILABLE_HUMANS) {
            if (g_world_unit_records[count].proposition_status != 0) {
                continue;
            }
            if (g_world_unit_records[count].gender_flags & UNIT_FLAG_EGG) {
                continue;
            }
            if (g_world_unit_records[count].uses_monster_skillset != 0) {
                continue;
            }
            if (job_is_special_monster(g_world_unit_records[count].identity.job_id) != 0) {
                continue;
            }
        } else if (mode == WORLD_FORMATION_RECORD_LIST_MONSTERS) {
            if (g_world_unit_records[count].gender_flags & UNIT_FLAG_EGG) {
                continue;
            }
            if ((g_world_unit_records[count].gender_flags >> 6) != 0) {
                continue;
            }
        }
        if (id > 0) {
            found = 0;
            for (i = 0; i < WORLD_EQUIPMENT_SLOT_COUNT; i++) {
                if (g_world_unit_records[count].equipment[i] == id) {
                    found = 1;
                }
            }
            if (found == 0) {
                continue;
            }
        }
        g_world_formation_record_order[count] = count;
        count++;
    }
    g_world_formation_record_count = count;
    world_formation_sort_roster(0);
    return g_world_formation_record_count;
}
