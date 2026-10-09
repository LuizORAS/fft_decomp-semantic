#include "fft/event_equip.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Write the edited loadout back to the unit's battle stats and party record,
 * refresh its stats, restore HP/MP up to the new maxima and rebuild the
 * EQUIP unit record. */
void equip_unit_commit_loadout_to_battle_stats(void) {
    world_item_stat_detail_t bonuses;
    battle_stats_t* stats;
    s16 unit_id;
    s32 item_id;
    s32 saved;
    s32 hp;
    s32 mp;
    s32 max;

    unit_id = g_equip_unit_data[0]->battle_id;
    stats = battle_unit_get_stats_from_battle_id(unit_id);
    item_id = g_equip_unit_data[0]->equipment[0];
    stats->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = 0xFF;
    stats->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_SHIELD] = 0xFF;
    if (item_id != 0) {
        if (g_main_item_primary_data[item_id].type == ITEM_TYPE_SHIELD) {
            stats->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_SHIELD] = item_id;
        } else {
            stats->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = item_id;
        }
    }
    item_id = g_equip_unit_data[0]->equipment[1];
    stats->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = 0xFF;
    stats->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_SHIELD] = 0xFF;
    if (item_id != 0) {
        if (g_main_item_primary_data[item_id].type == ITEM_TYPE_SHIELD) {
            stats->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_SHIELD] = item_id;
        } else {
            stats->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = item_id;
        }
    }
    stats->equipment[UNIT_EQUIPMENT_SLOT_HEAD]
        = g_equip_unit_data[0]->equipment[2] != 0 ? g_equip_unit_data[0]->equipment[2] : 0xFF;
    stats->equipment[UNIT_EQUIPMENT_SLOT_BODY]
        = g_equip_unit_data[0]->equipment[3] != 0 ? g_equip_unit_data[0]->equipment[3] : 0xFF;
    stats->equipment[UNIT_EQUIPMENT_SLOT_ACCESSORY]
        = g_equip_unit_data[0]->equipment[4] != 0 ? g_equip_unit_data[0]->equipment[4] : 0xFF;
    bcopy(stats->equipment, main_party_get_data_pointer(g_equip_unit_data[0]->formation_index_78)->equipment, 7);
    main_unit_refresh_stats_and_statuses(stats);
    stats = battle_unit_get_stats_from_battle_id(unit_id);
    hp = g_equip_unit_saved_hp;
    mp = g_equip_unit_saved_mp;
    max = stats->max_hp;
    if (hp > max) {
        hp = max;
    }
    stats->hp = hp;
    max = stats->max_mp;
    stats->mp = mp > max ? max : mp;
    /* The callee's parameter is the provisional 0x80-byte view of this 0x40-byte record. */
    equip_unit_calculate_equipment_stat_bonuses((equip_stats_t*)&bonuses, g_equip_unit_data[0]->equipment);
    stats->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_RIGHT_WEAPON_POWER] = bonuses.right_weapon_power;
    stats->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_LEFT_WEAPON_POWER] = bonuses.left_weapon_power;
    stats->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_RIGHT_WEAPON_EVADE] = bonuses.right_weapon_evade;
    stats->equipment_stats[BATTLE_UNIT_EQUIPMENT_STAT_LEFT_WEAPON_EVADE] = bonuses.left_weapon_evade;
    saved = g_equip_unit_data[0]->unit_count;
    equip_unit_build_data_from_battle_stats(stats, g_equip_unit_data[0]);
    g_equip_unit_data[0]->unit_count = saved;
    g_equip_unit_data[0]->battle_id = unit_id;
}
