#include "fft/wldcore.h"

/* Location script opcode 4 (HasUnit): fail the script when no roster unit has the operand's sprite
 * set. */
void wldcore_script_check_roster_has_sprite_set(void) {
    s32 i;
    s32 sprite_set;
    u16* ipp;
    u16 ip;
    party_unit_t* unit;
    s32* flags;

    ipp = &g_wldcore_script_state.ip;
    ip = *ipp;
    sprite_set = g_wldcore_script_state.data[ip];
    *ipp = ip + 1;
    for (i = 0; i < PARTY_ROSTER_SLOT_COUNT; i++) {
        unit = wldcore_unit_get_party_unit(i);
        if (unit->party_id != PARTY_ID_NONE) {
            if (unit->sprite_set == sprite_set) {
                break;
            }
        }
    }
    if (i == PARTY_ROSTER_SLOT_COUNT) {
        flags = &g_wldcore_script_state.flags;
        *flags |= WLDCORE_SCRIPT_RESULT_CONDITION_FAILED;
    }
}
