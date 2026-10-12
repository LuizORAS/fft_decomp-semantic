#include "fft/battle.h"
#include "fft/battle_script.h"
#include "fft/battle_text.h"

/* EVENT_SCRIPT_VAR_RANDOM_VALUE is read through the event
 * RNG, not its ordinary storage. ZERO never reads the uninitialized operand.
 * Keep the target's arithmetic edge behavior: only division explicitly stops
 * on zero; modulo has no such check, and signed overflow is not normalized. */
void script_run_variable_command(s32 opcode, s32 destination_id, s32 source, s32 unused) {
    s32* source_word;
    s32* destination_word;
    s32 write_allowed;
    s32 source_bit;
    s32 destination_bit;
    u32 shifted_mask;
    u32 mask;
    s32 operand;
    u32 preserve_mask;
    u32 replay_mode;
    s32 value;

    if (opcode != EVENT_OPCODE_ZERO_VARIABLE) {
        if (!(opcode & 1)) {
            operand = source;
        } else if (source == EVENT_SCRIPT_VAR_RANDOM_VALUE) {
            operand = script_get_rand16();
        } else {
            source_word = script_get_variable_word_pointer_from_id(source);
            source_bit = script_get_variable_bit_position(source);
            if (source_bit >= 0) {
                mask = 1;
                if (source >= EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) {
                    mask = 0xf;
                }
                operand = ((mask << source_bit) & *source_word) >> source_bit;
            } else {
                operand = *source_word;
            }
        }
    }
    preserve_mask = -1;
    destination_word = script_get_variable_word_pointer_from_id(destination_id);
    destination_bit = script_get_variable_bit_position(destination_id);
    if (destination_bit >= 0) {
        mask = 1;
        if (destination_id >= EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) {
            mask = 0xf;
        }
        shifted_mask = mask << destination_bit;
        preserve_mask = ~shifted_mask;
        value = (shifted_mask & *destination_word) >> destination_bit;
    } else {
        value = *destination_word;
    }
    if (opcode == EVENT_OPCODE_ZERO_VARIABLE) {
        value = 0;
    } else if (opcode < EVENT_OPCODE_SUBTRACT_IMMEDIATE) {
        value += operand;
    } else if (opcode < EVENT_OPCODE_MULTIPLY_IMMEDIATE) {
        value -= operand;
    } else if (opcode < EVENT_OPCODE_DIVIDE_IMMEDIATE) {
        value *= operand;
    } else if (opcode < EVENT_OPCODE_MODULO_IMMEDIATE) {
        if (operand == 0) {
            battle_thread_exit_current();
        }
        value = value / operand;
    } else if (opcode < EVENT_OPCODE_AND_IMMEDIATE) {
        value = value % operand;
    } else if (opcode < EVENT_OPCODE_OR_IMMEDIATE) {
        value &= operand;
    } else if (opcode < EVENT_OPCODE_ZERO_VARIABLE) {
        value |= operand;
    }
    /* Guard recursive getter calls: replay mode and saturated high variables
     * can suppress writes, but the nested getter must still read its operand. */
    if (g_script_variable_write_guard == 0) {
        if (destination_id >= EVENT_SCRIPT_VAR_PERSON_PROFILE_FIRST) {
            g_script_variable_write_guard = 1;
            write_allowed = (u32)script_get_variable(destination_id) < 12;
            g_script_variable_write_guard = 0;
            if (!write_allowed) {
                return;
            }
        }
        if (g_script_variable_write_guard == 0) {
            g_script_variable_write_guard = 1;
            replay_mode = script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS);
            g_script_variable_write_guard = 0;
            if (replay_mode != 0 && (destination_id - EVENT_SCRIPT_VAR_SCENARIO_LOCAL_FIRST) >= 0x20U
                && (destination_id - EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) >= 4U
                && destination_id != EVENT_SCRIPT_VAR_TEXT_STORE_INDEX
                && (destination_id - EVENT_SCRIPT_VAR_CURRENT_ENTD) >= 8U
                && destination_id >= EVENT_SCRIPT_VAR_WAR_FUNDS && destination_id != EVENT_SCRIPT_VAR_MAP_ARRANGEMENT
                && (destination_id - EVENT_SCRIPT_VAR_TUTORIAL_WAIT_VALUE) >= 5U
                && destination_id != EVENT_SCRIPT_VAR_MUTE_TEXT_AUDIO_CUE) {
                return;
            }
        }
    }
    if (destination_bit >= 0) {
        mask = 1;
        if (destination_id >= EVENT_SCRIPT_VAR_PACKED_NIBBLE_FIRST) {
            mask = 0xf;
        }
        value &= mask;
        value <<= destination_bit;
        *destination_word = value | (preserve_mask & *destination_word);
        return;
    }
    *destination_word = value;
    if (destination_id == EVENT_SCRIPT_VAR_TEXT_SUBSTITUTION_VALUE) {
        g_text_substitution_values[0] = value;
    }
}
