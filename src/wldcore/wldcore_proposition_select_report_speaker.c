/* Proposition speaker pick
 * (wldcore_proposition_select_report_speaker-wldcore_proposition_adjust_message_index). Returns the only participant,
 * else the one with the highest/lowest bravery (mode 1/2) or faith (3/4) by the u16 mode at 0x8009f312 (unpacked
 * proposition row 0x8009f2ec + 0x26). The goto form for the shared bravery/faith tails matches the target, which
 * reloads the stat after the compare. */
#include "fft/wldcore.h"

/* Pick the unit who reports a returned proposition: the only one sent, or, by the row's speaker
 * mode, the one with the highest or lowest Brave (WLDCORE_PROPOSITION_SPEAKER_HIGHEST_BRAVERY,
 * _LOWEST_BRAVERY) or Faith (_HIGHEST_FAITH, _LOWEST_FAITH). Returns its roster index; another mode
 * with several units returns an unset value (see QUIRKS.md).
 *
 * The goto form for the shared bravery/faith tails matches the target, which
 * reloads the stat after the compare. */
s32 wldcore_proposition_select_report_speaker(s32 index) {
    party_data_t* unit;
    s32 best;
    s32 i;
    s32 selected;

    if (g_main_active_propositions[index].participant_count == 1) {
        return g_main_active_propositions[index].participant_indices[0];
    }
    best
        = (g_wldcore_selected_proposition_row[0].fields.speaker_mode == WLDCORE_PROPOSITION_SPEAKER_HIGHEST_BRAVERY
              || g_wldcore_selected_proposition_row[0].fields.speaker_mode == WLDCORE_PROPOSITION_SPEAKER_HIGHEST_FAITH)
        ? -1
        : 99999;
    for (i = 0; i < g_main_active_propositions[index].participant_count; i++) {
        unit = wldcore_unit_get_party_data_pointer(g_main_active_propositions[index].participant_indices[i]);
        switch (g_wldcore_selected_proposition_row[0].fields.speaker_mode) {
        case WLDCORE_PROPOSITION_SPEAKER_HIGHEST_BRAVERY:
            if (best < unit->bravery) {
                goto take_bravery;
            }
            break;
        case WLDCORE_PROPOSITION_SPEAKER_LOWEST_BRAVERY:
            if (unit->bravery < best) {
            take_bravery:
                best = unit->bravery;
                selected = g_main_active_propositions[index].participant_indices[i];
            }
            break;
        case WLDCORE_PROPOSITION_SPEAKER_HIGHEST_FAITH:
            if (best < unit->faith) {
                goto take_faith;
            }
            break;
        case WLDCORE_PROPOSITION_SPEAKER_LOWEST_FAITH:
            if (unit->faith < best) {
            take_faith:
                best = unit->faith;
                selected = g_main_active_propositions[index].participant_indices[i];
            }
            break;
        }
    }
    return selected;
}
