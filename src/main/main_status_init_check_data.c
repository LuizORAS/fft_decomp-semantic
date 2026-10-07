#include "fft/main.h"

/* Build the status check sets (g_main_status_check_sets, main_status_check_set_e) from the status
 * data flags: KO, lost, CT freeze, no reactions, immortal and formation immunity, event-excluded,
 * mount removal and the two sets nothing reads; plus the fixed unmountable set (Crystal, Dead,
 * Petrify, Blood Suck, Treasure, Berserk, Chicken, Frog, Charm). It also clears the Move-Find flags
 * (g_main_item_location_flags); the game reset (main_boot_reset_game_state) calls it. */
void main_status_init_check_data(void) {
    int status;
    u8 flags4;
    u8 flags5;
    int byte;
    u8 mask;

    main_util_clear_byte_data(g_main_item_location_flags, 0x80);
    main_util_clear_byte_data(&g_main_status_check_sets[0][0], MAIN_STATUS_CHECK_SET_COUNT * BATTLE_STATUS_BYTE_COUNT);
    status = 0;
    do {
        flags4 = g_main_status_effect_data[status].flags_1;
        flags5 = g_main_status_effect_data[status].flags_2;
        byte = status / 8;
        mask = 0x80 >> (status & 7);
        if (flags4 & STATUS_EFFECT_FLAG_1_KO)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_KO][byte] |= mask;
        if (flags4 & STATUS_EFFECT_FLAG_1_LOST)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_LOST][byte] |= mask;
        if (flags4 & STATUS_EFFECT_FLAG_1_UNUSED_SET_2)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_UNUSED_2][byte] |= mask;
        if (flags4 & STATUS_EFFECT_FLAG_1_FREEZE_CT)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_FREEZE_CT][byte] |= mask;
        if (flags5 & STATUS_EFFECT_FLAG_2_PREVENT_REACTION)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_PREVENT_REACTION][byte] |= mask;
        if (flags5 & STATUS_EFFECT_FLAG_2_IMMORTAL_IMMUNITY)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_IMMORTAL_IMMUNITY][byte] |= mask;
        if (flags5 & STATUS_EFFECT_FLAG_2_FORMATION_IMMUNITY)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_FORMATION_IMMUNITY][byte] |= mask;
        if (flags5 & STATUS_EFFECT_FLAG_2_UNUSED_SET_7)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_UNUSED_7][byte] |= mask;
        if (flags5 & STATUS_EFFECT_FLAG_2_EVENT_EXCLUDED)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_EVENT_EXCLUDED][byte] |= mask;
        if (flags5 & STATUS_EFFECT_FLAG_2_MOUNT_REMOVAL)
            g_main_status_check_sets[MAIN_STATUS_CHECK_SET_MOUNT_REMOVAL][byte] |= mask;
        status++;
    } while (status < BATTLE_STATUS_COUNT);
    g_main_status_check_sets[MAIN_STATUS_CHECK_SET_UNMOUNTABLE][BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
        = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD);
    g_main_status_check_sets[MAIN_STATUS_CHECK_SET_UNMOUNTABLE][BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
        = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BLOOD_SUCK)
        | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE);
    g_main_status_check_sets[MAIN_STATUS_CHECK_SET_UNMOUNTABLE][BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_BERSERK)]
        = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BERSERK) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHICKEN)
        | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG);
    g_main_status_check_sets[MAIN_STATUS_CHECK_SET_UNMOUNTABLE][BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARM)]
        = BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARM);
}
