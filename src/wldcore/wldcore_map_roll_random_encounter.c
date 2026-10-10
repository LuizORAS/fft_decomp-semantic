#include "fft/wldcore.h"

/* One random-encounter tier: the story-progress threshold that selects it,
 * the chance an encounter happens (a roll of 0..99 at or below it), and the
 * high-bit-first mask of which of the entry's eight ENTDs may be drawn. */
typedef struct wldcore_encounter_tier {
    u8 progress_threshold; /* 0x00 */
    u8 encounter_chance;   /* 0x01 */
    u8 encounter_mask;     /* 0x02 */
} wldcore_encounter_tier_t;

/* One road into a world-map location. Three per location, 19 locations
 * (ids 24..42), reached through the pointer cell g_wldcore_random_battle_data. */
typedef struct wldcore_encounter_entry {
    u8 route;                          /* 0x00 */
    u8 deployment_squad_id;            /* 0x01: deployment zone id = this + 0x200 */
    wldcore_encounter_tier_t tiers[4]; /* 0x02 */
    u8 entd_ids[8];                    /* 0x0e */
    u8 battle_map_id;                  /* 0x16 */
    u8 script_variable_id;             /* 0x17 */
} wldcore_encounter_entry_t;

extern wldcore_encounter_entry_t (*g_wldcore_random_battle_data)[3];

#define ENTRY_AT(byte_offset)   ((wldcore_encounter_entry_t*)((byte_offset) + (s32)base))
#define ENTRY_BASE(byte_offset) ((wldcore_encounter_entry_t*)((s32)base + (byte_offset)))

/* Roll for a random battle on one road into world-map location 24..42.
 *
 * The tier is the first of the road's four whose progress threshold is at or
 * above the road's script variable. A tier chance of 0 never triggers;
 * otherwise a battle happens when a roll of 0..99 is at or below the chance,
 * which is chance + 1 in 100. One of the ENTDs enabled in the tier's mask is
 * then drawn evenly. Returns 1 with the ENTD and map ids set; the squad id is
 * set as soon as the road is found. */
s32 wldcore_map_roll_random_encounter(s32 location, s32 route) {
    wldcore_encounter_entry_t(*base)[3];
    s32 offset;
    wldcore_encounter_entry_t* row;
    wldcore_encounter_entry_t* rolled;
    s32 progress;
    s32 chance;
    s32 mask;
    s32 count;
    s32 pick;

    location -= WORLD_LOCATION_FIELD_FIRST;
    if (location < 0 || location >= WORLD_LOCATION_FIELD_COUNT) {
        return 0;
    }
    base = g_wldcore_random_battle_data;
    offset = location * 72;
    for (location = 0; location < 3; location++) {
        if (ENTRY_BASE(offset + location * 24)->route == route) {
            break;
        }
    }
    if (location == 3) {
        return 0;
    }
    offset += location * 24;
    row = ENTRY_AT(offset);
    g_wldcore_random_battle_squad_id = row->deployment_squad_id + 0x200;
    progress = world_script_get_variable(row->script_variable_id);
    for (location = 0; location < 4; location++) {
        if (ENTRY_AT(offset + location * 3)->tiers[0].progress_threshold >= progress) {
            break;
        }
    }
    if (location == 4) {
        return 0;
    }
    rolled = ENTRY_AT(location * 3 + offset);
    chance = rolled->tiers[0].encounter_chance;
    if (chance == 0) {
        return 0;
    }
    pick = rand() * 100 >> 15;
    if (chance >= pick) {
        mask = rolled->tiers[0].encounter_mask;
        count = 0;
        for (location = 0; location < 8; location++) {
            if ((mask >> location) & 1) {
                count++;
            }
        }
        pick = (rand() * count >> 15) + 1;
        for (location = 0; location < 8; location++) {
            if ((mask >> location) & 1) {
                pick--;
                if (pick == 0) {
                    break;
                }
            }
        }
        /* Typed indexing folds the base and index differently; derive the byte index from the last ENTD. */
        g_wldcore_random_battle_entd_id
            = ((u8*)base)[offset + (u32) & ((wldcore_encounter_entry_t*)0)->entd_ids[7] - location];
        g_wldcore_random_battle_map_id = ENTRY_AT(offset)->battle_map_id;
        return 1;
    }
    return 0;
}
