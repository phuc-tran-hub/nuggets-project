/*
 * grid.c
 * see grid.h for detailed documentation
 * 
 * Stanley Gao '24, CS50 21F, Nuggets Project
 * Ethan Thomas, 2021
 */

#include "compress.h"

static char GOLD_CHAR = '*';

static grid_t* findVisibleGold(game_t* game, player_t* targetPlayer, gold_t** goldArray);
static grid_t* findVisiblePlayers(player_t* targetPlayer, player_t** playerArray);

/* ****************** compress main ****************** */
// see compress.h for detailed documentation
grid_t*
compress_doCompression(game_t* game, player_t* targetPlayer, gold_t** goldArray, player_t** playerArray)
{
  grid_t* goldGrid = findVisibleGold(game, targetPlayer, goldArray);
  grid_t* playerGrid = findVisiblePlayers(targetPlayer, playerArray);
  grid_t* compressedGrid = grid_makeEmptyGrid(grid_getXLength(player_getGrid(targetPlayer)), grid_getYLength(player_getGrid(targetPlayer)));

  grid_union(compressedGrid, player_getGrid(targetPlayer));
  grid_union(compressedGrid, goldGrid);
  grid_union(compressedGrid, playerGrid);
  
  grid_freeGrid(goldGrid);
  grid_freeGrid(playerGrid);
  return compressedGrid;
}

/* ****************** spectatorCompression ****************** */
// see compress.h for detailed documentation
grid_t*
compress_spectatorCompression(game_t* game, gold_t** goldArray, player_t** playerArray)
{
  // make the gold grid
  grid_t* goldGrid = grid_makeEmptyGrid(game_getXLen(game), game_getYLen(game));

  for (int i = 0; i < game_getNumPiles(game); i++) {
    gold_t* g = goldArray[i];
    int x = gold_getX(g);
    int y = gold_getY(g);
    if (gold_getGold(g) != 0) {
      grid_setSpot(goldGrid, x, y, '*');
    }
  }

  // make the player grid
  grid_t* playerGrid = grid_makeEmptyGrid(game_getXLen(game), game_getYLen(game));

  int j = 0;
  while (playerArray[j] != NULL) {
    player_t* p = playerArray[j];
    int x = player_getX(p);
    int y = player_getY(p);
    char ID = player_getID(p);
    grid_setSpot(playerGrid, x, y, ID);
    j++;
  }

  // combine grids
  grid_t* compressedGrid = grid_makeEmptyGrid(game_getXLen(game), game_getYLen(game));

  grid_union(compressedGrid, game_getMaster(game));
  grid_union(compressedGrid, goldGrid);
  grid_union(compressedGrid, playerGrid);

  grid_freeGrid(goldGrid);
  grid_freeGrid(playerGrid);

  return compressedGrid;
}

/* helper functions */
// func to calculate visible gold to the player
static grid_t*
findVisibleGold(game_t* game, player_t* targetPlayer, gold_t** goldArray)
{
  grid_t* grid = grid_makeEmptyGrid(grid_getXLength(player_getGrid(targetPlayer)), grid_getYLength(player_getGrid(targetPlayer)));
  grid_t* pGrid = player_getGrid(targetPlayer);
  int pX = player_getX(targetPlayer);
  int pY = player_getY(targetPlayer);
  for (int i = 0; i < game_getNumPiles(game); i++) {
    gold_t* gold = goldArray[i];
    if (gold_getGold(gold) == 0) {
      continue;
    }
    int gX = gold_getX(gold);
    int gY = gold_getY(gold);
    if (visibility_calculateDirectVisibility(pGrid, pX, pY, gX, gY)) {
      grid_setSpot(grid, gX, gY, GOLD_CHAR);
    }
  }

  return grid;
}

// func to create a grid object of visible players to the player
static grid_t*
findVisiblePlayers(player_t* targetPlayer, player_t** playerArray)
{
  grid_t* grid = grid_makeEmptyGrid(grid_getXLength(player_getGrid(targetPlayer)), grid_getYLength(player_getGrid(targetPlayer)));
  grid_t* pGrid = player_getGrid(targetPlayer);
  int pX = player_getX(targetPlayer);
  int pY = player_getY(targetPlayer);
  for (int i = 0; i < 26; i++) {
    player_t* player = playerArray[i];
    if (player == NULL) {
      continue;
    }
    int p2X = player_getX(player);
    int p2Y = player_getY(player);
    if (visibility_calculateDirectVisibility(pGrid, pX, pY, p2X, p2Y)) {
      grid_setSpot(grid, p2X, p2Y, player_getID(player));
    }
  }
  grid_setSpot(grid, pX, pY, player_getID(targetPlayer));
  return grid;
}

