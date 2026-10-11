#include "fft/open.h"

/* Load WORLD if needed, then initialize the selected new-game party. */
void open_title_init_new_game_party(s32 party_mode, s32 world_load_mode) {
    u32 flags;
    u32 party_mode_flag;

    if ((g_open_system_runtime_flags & OPEN_SYSTEM_RUNTIME_FLAG_WORLD_OVERLAY_LOADED) == 0) {
        g_open_system_runtime_flags |= OPEN_SYSTEM_RUNTIME_FLAG_WORLD_OVERLAY_LOADED;
        main_overlay_load_world_bin(world_load_mode);
    }

    party_mode_flag = OPEN_PARTY_MODE_FLAG_BASE << party_mode;
    flags = g_open_system_runtime_flags;
    if ((flags & OPEN_SYSTEM_RUNTIME_FLAG_PARTY_CHOSEN) == 0 || (flags & party_mode_flag) == 0) {
        g_open_system_runtime_flags
            = ((flags | OPEN_SYSTEM_RUNTIME_FLAG_PARTY_CHOSEN) & ~OPEN_PARTY_MODE_FLAG_MASK) | party_mode_flag;
        main_party_init_new_game(party_mode);
        world_text_generate_formation_unit_name_string();
    }
}
