#include "fft/main.h"

/* Aggregate assignment changes the retail byte-copy loop. */
void main_util_copy_action_data(const battle_action_command_t* source, battle_action_command_t* destination) {
    const u8* from = (const u8*)source;
    u8* to = (u8*)destination;
    u32 index = 0;

    do {
        *to++ = *from++;
        index++;
    } while (index < sizeof(battle_action_command_t));
}
