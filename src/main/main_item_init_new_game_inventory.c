#include "fft/main.h"
#include "psx/types.h"

/* Empty the inventory, then stock a new game's items: 5 Potions, 2 Hi-Potions, 2 Eye Drops,
 * 2 Phoenix Downs, and one each of X-Potion, Ether, Hi-Ether, Elixir, Antidote, Echo Grass, Maiden's
 * Kiss, Soft, Holy Water and Remedy. */
void main_item_init_new_game_inventory(void) {
    int remaining = ITEM_ID_COUNT - 1;
    u8* quantity = &g_main_item_quantities[ITEM_ID_COUNT - 1];

    do {
        *quantity = 0;
        remaining--;
        quantity--;
    } while (remaining >= 0);

    g_main_item_quantities[ITEM_ID_POTION] = 5;
    g_main_item_quantities[ITEM_ID_HI_POTION] = 2;
    g_main_item_quantities[ITEM_ID_X_POTION] = 1;
    g_main_item_quantities[ITEM_ID_ETHER] = 1;
    g_main_item_quantities[ITEM_ID_HI_ETHER] = 1;
    g_main_item_quantities[ITEM_ID_ELIXIR] = 1;
    g_main_item_quantities[ITEM_ID_ANTIDOTE] = 1;
    g_main_item_quantities[ITEM_ID_EYE_DROP] = 2;
    g_main_item_quantities[ITEM_ID_ECHO_GRASS] = 1;
    g_main_item_quantities[ITEM_ID_MAIDENS_KISS] = 1;
    g_main_item_quantities[ITEM_ID_SOFT] = 1;
    g_main_item_quantities[ITEM_ID_HOLY_WATER] = 1;
    g_main_item_quantities[ITEM_ID_REMEDY] = 1;
    g_main_item_quantities[ITEM_ID_PHOENIX_DOWN] = 2;
}
