#include "fft/main.h"

/* Copy a 20-byte unit or AI action command. */
void main_util_copy_action_data(const void* source, void* destination) {
    const u8* from = source;
    u8* to = destination;
    u32 index = 0;

    do {
        *to++ = *from++;
        index++;
    } while (index < sizeof(battle_unit_action_command_t));
}
