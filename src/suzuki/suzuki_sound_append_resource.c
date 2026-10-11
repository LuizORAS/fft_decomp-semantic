#include "psx/suzuki.h"
#include "psx/types.h"

/* Appends a loaded SED/SMD block to the end of the Suzuki driver's sound
 * resource list. */
void suzuki_sound_append_resource(suzuki_sound_resource_t* resource) {
    suzuki_sound_resource_t** slot;
    suzuki_sound_resource_t* node;

    slot = &g_suzuki_sound_resource_list;
    if (g_suzuki_sound_resource_list != 0) {
        do {
            node = *slot;
            slot = &node->next;
        } while (node->next != 0);
    }
    *slot = resource;
    resource->next = 0;
}
