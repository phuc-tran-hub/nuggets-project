/*
 * compress.h: implementation of grid compression for game
 *
 * Stanley Gao '24, CS50 21F, Nuggets Project
 * Ethan Thomas, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "player.h"
#include "grid.h"
#include "gold.h"
#include "visibility.h"
#include "game.h"

/* ****************** compress main ****************** */
/*
 * given target player (whcih includes its visible grid), and an array of gold/player objects,
 * calculate a compressed grid that we can send to the player
 */
grid_t* compress_doCompression(game_t* game, player_t* targetPlayer, gold_t** goldArray, player_t** playerArray);

/* ****************** spectatorCompression ****************** */
/*
 * compress grids for the spectators view
 */
grid_t* compress_spectatorCompression(game_t* game, gold_t** goldArray, player_t** playerArray);
