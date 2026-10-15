#include "psx/types.h"

/* Copy 20 bytes, the size of a command action record
 * (battle_ai_command_action_t). */
void main_util_copy_action_data(const void* source, void* destination) {
    const u8* from = source;
    u8* to = destination;
    u32 index = 0;

    do {
        *to++ = *from++;
        index++;
    } while (index < 20);
}
