#include "fft/battle_ai.h"

/* Target 0x8019ae30. */
void ai_init_selected_action(void) {
    ai_reset_action_ranking_fields(&g_ai_data_base.selected_action);
}
