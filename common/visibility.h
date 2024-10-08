/*
 * visibility.h - module for calculating the visibility of a player in a grid
 *
 * Ethan Thomas, 2021
 * Stanley Gao '24, CS50 21F
 */

#ifndef __VISIBILITY_H
#define __VISIBILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grid.h"
#include "player.h"

/************** visibility_calculateDirectVisibility **************/
/*
 * determine, using a mapgrid and two points (two sets of (x,y) coordinates),
 * if the object at (x1,y1) can directly see (x2,y2)
 * 
 * in the case of nuggets, used to check player-gold and player-player visibility (direct line of sight)
 */
bool visibility_calculateDirectVisibility(grid_t* mapGrid, int x1, int y1, int x2, int y2);

/************** visibility_calculatePlayerVisibility **************/
/*
 * generate, using a master mapgrid and a player (an object with (x,y) coordinates),
 * a mapgrid of only what the player can directly see.
 * using a vision algorithm, this function returns a subset of the master mapgrid 
 * 
 * in the case of nuggets, used to create a current-vision map which gets merged with past vision (for vision history)
 */
grid_t* visibility_calculatePlayerVisibility(grid_t* mapGrid, player_t* player);


#endif // __VISIBILITY_H
