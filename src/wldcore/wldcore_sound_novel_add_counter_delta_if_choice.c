#include "fft/wldcore.h"
#include "psx/types.h"

/* Scalar views of the instruction word and of the u16 counter at 0x800d4852
 * inside wldcore_sound_novel_state_t: neither `*(u16*)&` nor a u16* variable over
 * the struct reproduced the target's counter access. */
extern u32 g_wldcore_sound_novel_instruction;
extern u16 g_wldcore_sound_novel_counter_delta;

/* Sound novel opcode 51: when the dialog option chosen (EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION)
 * equals the operand's third byte, add the signed middle halfword of the instruction word to the
 * record's counter delta (g_wldcore_sound_novel_counter_delta).
 *
 * The target keeps &g_wldcore_sound_novel_instruction in $s1 across the call and reaches both fields
 * off it (`lbu 3(s1)` / `lw 0(s1)`); every plain pointer, union and bitfield spelling lets cse fold
 * the address back into the load's %lo displacement. Laundering the constant address through a
 * non-volatile asm hides it from cse and emits nothing. */
void wldcore_sound_novel_add_counter_delta_if_choice(void) {
    u32* instruction;
    u8 expected;

    __asm__("" : "=r"(instruction) : "0"(&g_wldcore_sound_novel_instruction));

    expected = ((u8*)instruction)[3];
    if (world_script_get_variable(EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION) == expected) {
        g_wldcore_sound_novel_counter_delta += (s32)((*instruction << 8) >> 16) - 0x8000;
    }
    g_wldcore_sound_novel_state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
