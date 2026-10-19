#include "fft/main.h"

/* Copy 20 bytes, the size of a command action record (action_command_t), one byte
 * at a time: an aggregate assignment changes the retail byte-copy loop. */
void main_util_copy_action_data(const action_command_t* source, action_command_t* destination) {
    const u8* from = (const u8*)source;
    u8* to = (u8*)destination;
    u32 index;

    for (index = 0; index < sizeof(*destination); index++) {
        *to++ = *from++;
    }
}
