/*
 * unit test for visibility.c
 *    hardcodes a player at a specific spot and calculates their visibleGrid
 *
 *    prints the mapGrid followed by the visibleGrid
 * 
 * Ethan Thomas, 2021
 * Stanley Gao '24, CS50 21F
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../player.h"
#include "../grid.h"
#include "../visibility.h"
#include "message.h"

// (3, 2) is in top-left room; (9,15) is in bottom-left room; (14, 2) is in a passage
// (59, 4) is in top-right room; (59, 13) is in bottom-right room
const int x = 9;
const int y = 15;

int
main()
{

  char* mapfile = "main.txt";

  grid_t* mapGrid = grid_loadGridFromFile(mapfile);

  grid_print(mapGrid, stdout);

  addr_t noAddr = message_noAddr();
  player_t* player = player_new('A', "ethan", noAddr, mapGrid, x, y);

  grid_t* visGrid = visibility_calculatePlayerVisibility(mapGrid, player);
  grid_setSpot(visGrid, x, y, 'A');

  printf("\n\n");

  for (int i = 0; i < 100; i++) {
    printf("@");
  }

  printf("\n\n\n");

  grid_print(visGrid, stdout);

  player_delete(player);
  grid_freeGrid(mapGrid);
  grid_freeGrid(visGrid);
  return 0; // exit status
}
