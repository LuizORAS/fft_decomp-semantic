#include "fft/battle.h"
#include "fft/main.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/sound.h"
#include "fft/thread.h"
#include "fft/world.h"

/*
 * WORLD twin of the BATTLE system-function dispatcher battle_menu_dispatch_system_function; see
 * that source for the command map. Callers reach it through
 * world_menu_init_system_function and world_text_determine_spell_quote.
 */
s32 world_menu_dispatch_system_function(
    s32 command, s32 option, s32 unit_id, s32 parameter, s32 enabled, action_command_t* action) {
    unit_stats_t* unit;
    s32 value;
    s32 count;
    s32 i;
    item_data_t* item;
    menu_system_function_t* entry;
    s32 flags;

    g_world_menu_thread_menu_data = g_world_menu_system_entries;
    g_world_menu_current_id = BATTLE_MENU_ID_NONE;
    g_menu_restore_pending = 0;
    unit = battle_unit_get_stats_from_battle_id(unit_id);
    if (command == BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK) {
        value = script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT);
        if (value >= EVENT_ID_TUTORIAL_FIRST && value <= EVENT_ID_TUTORIAL_LAST) {
            if (option != 2) {
                world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
            }
            return MENU_SYSTEM_DISPATCH_DONE;
        } else if (g_world_menu_input_mode != MENU_INPUT_PLAYER) {
            if (option != 2) {
                world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
            }
            return MENU_SYSTEM_DISPATCH_DONE;
        } else if (option != 1) {
            if (world_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) != 0) {
                main_system_report_stub(7, 3);
                if (option != 2) {
                    world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
                }
                return MENU_SYSTEM_DISPATCH_DONE;
            } else {
                count = 0;
                g_script_condition_unit_battle_id = unit_id;
                for (i = 0; i < 3; i++) {
                    if (battle_unit_find_slot_by_character_identity(i + 1) == BATTLE_CHARACTER_SLOT_UNAVAILABLE) {
                        count++;
                    }
                }
                if ((action_check_battle_outcome() < 0 || count != 0)
                    && script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) == 0) {
                    if (option != 2) {
                        script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, EVENT_ID_GAME_OVER);
                        world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
                        g_world_menu_overlay_state = 1;
                        g_world_menu_scenario_event_started = 1;
                        world_script_start_current_event_thread();
                    } else {
                        g_world_finish_operation_event_id = EVENT_ID_GAME_OVER;
                    }
                    return MENU_SYSTEM_DISPATCH_EVENT;
                }
                if (g_world_menu_scenario_event_started != 0) {
                    if (script_run_scenario_conditions() != 0) {
                        if (option != 2) {
                            world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
                            g_world_menu_overlay_state = 1;
                            world_script_start_current_event_thread();
                        } else {
                            g_world_finish_operation_event_id = script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT);
                            script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, value);
                        }
                        return MENU_SYSTEM_DISPATCH_EVENT;
                    }
                    if (option != 2) {
                        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
                    }
                    return MENU_SYSTEM_DISPATCH_DONE;
                } else if (script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) != 0) {
                    if (option != 2) {
                        world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
                        g_world_menu_overlay_state = 1;
                        if (script_get_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE) != 0) {
                            script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, EVENT_ID_DEBUG_BATTLE_START);
                        } else {
                            script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, EVENT_ID_BATTLE_START);
                        }
                        world_script_start_current_event_thread();
                    } else {
                        g_world_finish_operation_event_id = EVENT_ID_BATTLE_START;
                    }
                    return MENU_SYSTEM_DISPATCH_EVENT;
                } else if (action_check_battle_outcome() == 0) {
                    if (option != 2) {
                        world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
                        g_world_menu_overlay_state = 1;
                        script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, EVENT_ID_BATTLE_WON);
                        g_world_menu_scenario_event_started = 1;
                        world_script_start_current_event_thread();
                    } else {
                        g_world_finish_operation_event_id = EVENT_ID_BATTLE_WON;
                    }
                    return MENU_SYSTEM_DISPATCH_EVENT;
                } else {
                    if (option != 2) {
                        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
                    }
                    return MENU_SYSTEM_DISPATCH_DONE;
                }
            }
        }
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
        return MENU_SYSTEM_DISPATCH_DONE;
    }
    if (enabled == 0) {
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
        return MENU_SYSTEM_DISPATCH_DONE;
    }
    if (command == BATTLE_MENU_SYSTEM_COMMAND_TARGETING_PROMPT) {
        value = 8;
        if (g_world_menu_resolved_command == 0 || g_world_menu_resolved_command == 0x19) {
            value = 7;
            if (option == 2) {
                value = 0x31;
            }
        } else if (g_world_menu_resolved_command == 1) {
            g_menu_restore_pending = 1;
            value = 6;
            if (option == 1) {
                value = 0x29;
            }
        } else if (g_world_menu_resolved_command == 0xC) {
            value = 0x14;
        } else if (g_world_menu_resolved_command == 0xE) {
            value = 0x15;
        } else if (g_world_menu_resolved_skillset == SKILLSET_ID_DEFEND) {
            value = 0x1C;
        }
        world_menu_start_system_function_thread(value);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK) {
        sound_queue_sfx_confirm();
        if (option == 0) {
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
        } else if (option == 1) {
            world_menu_start_system_function_thread(0);
        } else if (option == 4) {
            world_menu_start_system_function_thread(0x30);
        } else {
            world_sound_queue_sfx_invalid();
            world_menu_start_system_function_thread(option + 0xD);
        }
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_WAIT_MENU) {
        if (g_world_menu_last_dispatched_command == BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK
            && g_world_menu_last_dispatched_option == 1) {
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
        } else {
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_WAIT_MENU);
        }
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM) {
        value = 0x2F;
        if (option != 5) {
            value = 0xB;
            if (option != 1) {
                if (option == 2 || option == 4) {
                    if (g_world_menu_resolved_ability == 0xFFFE || g_world_ability_selected_skill_ct == 0) {
                        value = 0x2D;
                    } else {
                        value = 0xC;
                    }
                } else {
                    if (g_world_menu_resolved_ability != 0xFFFE || g_world_ability_selected_skill_ct == 0) {
                        value = 0x2E;
                    } else {
                        value = 0xD;
                    }
                }
            }
        }
        g_menu_restore_pending = 1;
        world_menu_start_system_function_thread(value);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_SPELL_QUOTE) {
        g_world_spell_quote_last_ability_id = action->ability_id & ABILITY_ID_MASK;
        thread_start(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, world_text_run_spell_quote_thread);
        thread_set_parameters(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, unit_id, g_world_spell_quote_last_ability_id, 0);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_ABILITY_ANNOUNCEMENT) {
        world_text_set_message_duration_frames(0x3C);
        if (g_main_action_menu_types_by_skillset[unit->action_command.skillset_id] == ACTION_MENU_TYPE_ARITHMETICKS) {
            g_text_substitution_values[1] = unit->action_command.calculator_multiplier + TEXT_ID_ABILITY_NAME_BASE;
            g_text_substitution_values[0] = unit->action_command.calculator_type + TEXT_ID_ABILITY_NAME_BASE;
            g_text_substitution_values[2] = unit->action_command.ability_id + TEXT_ID_ABILITY_NAME_BASE;
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_ARITHMETICKS_ANNOUNCEMENT);
        } else {
            if (action->skillset_id == SKILLSET_ID_ITEM || action->skillset_id == SKILLSET_ID_THROW) {
                g_text_substitution_values[0] = (u8)action->item_or_equipment + TEXT_ID_ITEM_NAME_BASE;
            } else {
                value = action->ability_id & ABILITY_ID_MASK;
                if (value >= ABILITY_ID_SUMMON_MAGIC_MOOGLE && value <= ABILITY_ID_DRAW_OUT_CHIRIJIRADEN) {
                    g_text_substitution_values[0] = value + 0xB7C4;
                } else {
                    g_text_substitution_values[0] = value + TEXT_ID_ABILITY_NAME_BASE;
                }
            }
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_ABILITY_ANNOUNCEMENT);
        }
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_CLOSE) {
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_TARGET_CONFIRM) {
        value = 8;
        if (option == 1) {
            value = 0x16;
        } else if (option == 2) {
            value = 0x17;
        } else if (option == 3) {
            value = 0x20;
        } else if (g_world_menu_resolved_command == 0xC) {
            value = 0x1A;
        } else if (g_world_menu_resolved_command == 0xE) {
            value = 0x1B;
        }
        world_menu_start_system_function_thread(value);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE) {
        if (g_main_game_options.fields.effect_messages != GAME_OPTION_ON) {
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
        } else {
            g_world_system_function_table[BATTLE_SYSTEM_FUNCTION_EFFECT_MESSAGE].text_id = option;
            g_text_substitution_values[0] = parameter & 0xFF;
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_EFFECT_MESSAGE);
        }
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_ABILITY_LEARNED) {
        g_text_substitution_values[0] = option + TEXT_ID_ABILITY_NAME_BASE;
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_ABILITY_LEARNED);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_CRYSTAL_OR_TREASURE) {
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CRYSTAL_OR_TREASURE);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM) {
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_MOVE_CONFIRM);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_TRAP_RESULT) {
        world_menu_start_system_function_thread(option + BATTLE_SYSTEM_FUNCTION_TRAP_RESULT_FIRST);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_MOVE_FIND_ITEM) {
        if (main_item_get_total_equipment_quantity(option & 0xFF, 1) == 0x63) {
            main_item_get_data_pointer(option);
            g_text_substitution_values[0] = option;
            g_text_substitution_values[1] = option;
            item = main_item_get_data_pointer(option);
            g_text_substitution_values[2] = item->price >> 2;
            script_set_variable(
                EVENT_SCRIPT_VAR_WAR_FUNDS, (item->price >> 2) + script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS));
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_MOVE_FIND_ITEM_SOLD);
        } else {
            main_item_get_data_pointer(option);
            g_text_substitution_values[0] = option;
            g_main_item_quantities[option]++;
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_MOVE_FIND_ITEM);
        }
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM_SILENT) {
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_MOVE_CONFIRM_SILENT);
    } else if (command == BATTLE_MENU_SYSTEM_COMMAND_POST_ACTION_RESULT) {
        entry = &g_world_system_function_table[BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT];
        flags = parameter;
        if (option >= ACTION_DISPLAY_IMAGE_CT && option <= ACTION_DISPLAY_IMAGE_LEVEL) {
            g_text_substitution_values[0] = unit_id;
            g_text_substitution_values[1] = option + 0x506B;
            if (option == ACTION_DISPLAY_IMAGE_LEVEL) {
                flags = 1;
                if (parameter & ACTION_STAT_CHANGE_INCREASE) {
                    flags = 1 | ACTION_STAT_CHANGE_INCREASE;
                }
            }
            g_text_substitution_values[2] = flags;
            if (flags & ACTION_STAT_CHANGE_INCREASE) {
                g_text_substitution_values[2] = flags & ACTION_STAT_CHANGE_VALUE_MASK;
                entry->text_id = 0x508C;
            } else {
                entry->text_id = 0x508D;
            }
            if (option == ACTION_DISPLAY_IMAGE_CT) {
                if (flags == 0xFF) {
                    entry->text_id = 0x509A;
                } else if (flags == 0x7F) {
                    entry->text_id = 0x5099;
                }
            }
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT);
        } else if (option == ACTION_DISPLAY_IMAGE_GIL) {
            g_text_substitution_values[0] = parameter < 0 ? -parameter : parameter;
            g_world_system_function_table[BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT].text_id = 0x508F;
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT);
        } else if (option == ACTION_DISPLAY_IMAGE_EXP) {
            g_text_substitution_values[0] = parameter & 0x7F;
            g_world_system_function_table[BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT].text_id = 0x508E;
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT);
        } else if (option == ACTION_DISPLAY_IMAGE_STOLEN || option == ACTION_DISPLAY_IMAGE_BROKEN) {
            if (parameter & 0x8000) {
                g_text_substitution_values[0] = (parameter & 0xFF) + TEXT_ID_ITEM_NAME_BASE;
                g_world_system_function_table[BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT].text_id = 0x509B;
            } else {
                g_text_substitution_values[0] = parameter + TEXT_ID_ITEM_NAME_BASE;
                g_world_system_function_table[BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT].text_id = option + 0x5074;
            }
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_POST_ACTION_RESULT);
        } else {
            world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
        }
    } else {
        main_system_report_stub(7, 5);
        world_menu_start_system_function_thread(BATTLE_SYSTEM_FUNCTION_CLOSE);
        g_world_menu_last_dispatched_command = command;
        g_world_menu_last_dispatched_option = option;
        return MENU_SYSTEM_DISPATCH_UNKNOWN_COMMAND;
    }
    g_world_menu_last_dispatched_command = command;
    g_world_menu_last_dispatched_option = option;
    return MENU_SYSTEM_DISPATCH_DONE;
}
