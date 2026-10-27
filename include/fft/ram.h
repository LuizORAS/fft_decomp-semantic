#ifndef RAM_H
#define RAM_H

/* The RAM window sizes and offsets the code compiles in, generated from target/ram.yaml by
 * `make config-fmt`: edit the map, not this file. */

/* ram */
/* The bytes from a window's start to where the next thing resident with it starts, and from one
 * window's start to another's. */

typedef enum ram_window_bytes {
    RAM_WINDOW_BYTES_WORLD = 0xf0000,
    RAM_WINDOW_BYTES_COMPANION = 0x20000,
    RAM_WINDOW_BYTES_HIGH = 0x20000,
} ram_window_bytes_e;

typedef enum ram_window_start {
    RAM_WINDOW_START_EFFECT_FROM_COMPANION = 0x3500,
} ram_window_start_e;

#endif
