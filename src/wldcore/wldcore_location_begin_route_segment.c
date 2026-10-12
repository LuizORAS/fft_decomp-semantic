#include "fft/wldcore.h"

/* Stages one world-map route segment in the location-entry state.
 *
 * The low byte of `route` is the route id; its bit 8 becomes flags bit 1 and
 * selects the direction, and `step` is the point to start from: the
 * descending arm counts back from the end of the record and takes the heading
 * (turned by 0x800) and distance from the preceding point; the ascending arm
 * takes them from the point itself. Returns the point count minus one.
 *
 * The table pointer and the record share one variable: that is what makes the
 * target load the table first and keep both in $t0. The unused 32-byte local
 * reproduces the target's otherwise empty 0x20 frame, and `index` is shared by
 * both arms while each arm has its own point pointer, which fixes the
 * target's register choice for both index computations. The packed 32-bit position values supply x in the low
 * 16 bits and y in the high 16 bits; the `flags` local gives the
 * target's `or` operand order. */
s32 wldcore_location_begin_route_segment(wldcore_location_entry_state_t* state, u32 route, s32 step) {
    s32 unused[8];
    wldcore_route_record_t* record;
    s32 index;
    wldcore_route_point_t* point_down;
    wldcore_route_point_t* point_up;
    s32 word;
    s32 flags;

    record = (wldcore_route_record_t*)g_wldcore_map_route_tables;
    state->route_id = route & 0xff;
    flags = state->flags;
    state->flags = flags | ((route >> 7) & WLDCORE_ROUTE_TRAVEL_REVERSED);
    record = ((wldcore_route_record_t**)record)[state->route_id];
    if (state->flags & WLDCORE_ROUTE_TRAVEL_REVERSED) {
        state->leg_end_location = record->start_location;
        state->leg_start_location = record->end_location;
        /* Direct point indexing moves the header addition past the shift. */
        index = (record->count - step - 1) * (sizeof(wldcore_route_point_t) / sizeof(u32))
            + offsetof(wldcore_route_record_t, points) / sizeof(u32);
        point_down = (wldcore_route_point_t*)(index * sizeof(u32) + (s32)record);
        word = point_down->position;
        state->position.vx = word;
        state->position.vy = (u16)(point_down->position >> 16);
        state->position.vz = 0;
        word = point_down[-1].position;
        state->target_position.vx = word;
        state->target_position.vy = (u16)(point_down[-1].position >> 16);
        state->target_position.vz = 0;
        state->previous_position = state->position;
        state->heading = ((point_down[-1].heading >> 16) + 0x800) & 0xfff;
        state->segment_frames = point_down[-1].distance * 2;
    } else {
        state->leg_start_location = record->start_location;
        state->leg_end_location = record->end_location;
        index = step * (sizeof(wldcore_route_point_t) / sizeof(u32))
            + offsetof(wldcore_route_record_t, points) / sizeof(u32);
        point_up = (wldcore_route_point_t*)(index * sizeof(u32) + (s32)record);
        word = point_up->position;
        state->position.vx = word;
        state->position.vy = (u16)(point_up->position >> 16);
        state->position.vz = 0;
        word = point_up[1].position;
        state->target_position.vx = word;
        state->target_position.vy = (u16)(point_up[1].position >> 16);
        state->target_position.vz = 0;
        state->previous_position = state->position;
        state->heading = (point_up->heading >> 16);
        state->segment_frames = point_up->distance * 2;
    }
    state->segment_frame = 0;
    return record->count - 1;
}
