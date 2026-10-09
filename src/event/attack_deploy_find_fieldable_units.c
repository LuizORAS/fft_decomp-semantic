#include "fft/event_attack.h"

#define ATTACK_NAME_BYTES 16
#define ATTACK_NAME_END   0xfe

void attack_deploy_find_fieldable_units(void) {
    s32 index;
    s32 name_index;
    u8* name_cursor;
    party_data_t* party_unit;

    name_cursor = g_battle_text_section_pointers[TEXT_SECTION_PARTY_UNIT_NAMES];
    g_battle_text_section_pointers[TEXT_SECTION_ROSTER_UNIT_NAMES]
        = g_attack_text_data + g_attack_text_section_offset_1;
    g_battle_text_section_pointers[TEXT_SECTION_JOB_NAMES] = g_attack_text_data + g_attack_text_section_offset_2;
    g_battle_text_section_pointers[TEXT_SECTION_SKILLSET_NAMES_COPY]
        = g_battle_text_section_pointers[TEXT_SECTION_SKILLSET_NAMES];
    g_attack_deploy_fieldable_unit_count = 0;

    for (index = 0; index < PARTY_ROSTER_SLOT_COUNT; index++) {
        party_unit = main_party_get_data_pointer(index);
        if (party_unit->party_id == PARTY_ID_NONE || index >= PARTY_GUEST_SLOT_FIRST
            || party_unit->proposition_status != 0 || (party_unit->gender_flags & UNIT_FLAG_EGG) != 0) {
            g_attack_deploy_unit_fieldable[index] = 0;
            *name_cursor++ = ATTACK_NAME_END;
        } else {
            g_attack_deploy_unit_fieldable[index] = 1;
            g_attack_deploy_fieldable_unit_count++;
            for (name_index = 0; name_index < ATTACK_NAME_BYTES; name_index++) {
                *name_cursor = party_unit->name[name_index];
                if ((*name_cursor & ATTACK_NAME_END) == ATTACK_NAME_END) {
                    name_cursor++;
                    break;
                }
                name_cursor++;
                if (name_index == ATTACK_NAME_BYTES - 1) {
                    *name_cursor++ = ATTACK_NAME_END;
                }
            }
        }
    }

    if (g_battle_scenario_event_active != 0) {
        return;
    }

    if (g_attack_deploy_squad_data[g_attack_deploy_current_squad_id].valid_tile_bitmap == 0) {
        g_attack_deploy_current_squad_id = 0;
    }

    if (battle_script_get_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE) != 0) {
        s32 map_id = battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_MAP);

        g_attack_deploy_current_squad_id = 0;
        for (index = 0; index < 0x300; index++) {
            if (g_attack_deploy_squad_data[index].map_id == map_id) {
                g_attack_deploy_current_squad_id = index;
                break;
            }
        }
        if (g_attack_deploy_squad_data[g_attack_deploy_current_squad_id].valid_tile_bitmap == 0) {
            g_attack_deploy_current_squad_id = 0;
        }
        battle_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SQUAD_COUNT, 2);
        battle_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_PRIMARY_SQUAD_ID, g_attack_deploy_current_squad_id);
        if (g_attack_deploy_current_squad_id == 0) {
            battle_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SECONDARY_SQUAD_ID, 0);
        } else {
            battle_script_set_variable(
                EVENT_SCRIPT_VAR_DEPLOYMENT_SECONDARY_SQUAD_ID, g_attack_deploy_current_squad_id + 1);
        }
        battle_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_UNIT_LIMIT,
            g_attack_deploy_squad_data[g_attack_deploy_current_squad_id].unit_limit);
        g_attack_deploy_squad_data[g_attack_deploy_current_squad_id].unit_limit = 4;
    } else {
        g_battle_entd_selection_mode = 0;
        g_attack_deploy_current_squad_id = battle_script_get_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_PRIMARY_SQUAD_ID);
        battle_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_UNIT_LIMIT,
            g_attack_deploy_squad_data[g_attack_deploy_current_squad_id].unit_limit);
    }
}
