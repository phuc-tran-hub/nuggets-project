/*
 * player.c -> see player.h for detailed documentation
 * 
 * Henry Bonnie   Nov. 7th   CS50 F21
 * Stanley Gao '24, CS50 21F
 */
#include "player.h"
#include "grid.h"
#include "message.h"
#include "compress.h"
//#include "visibility.h"

/* ****************** player ****************** */ 
/* see player.h for detailed documentation */
typedef struct player {
  char* playerName; // players name
  char playerID; // players character ID
  addr_t address;
  int x; // x location on grid
  int y; // y location on grid
  int purse; // players gold purse
  grid_t* visibleGrid; // players visible grid
} player_t;

/* ****************** getter functions ****************** */
/* getX */
int player_getX(player_t* player) {
  return player ? player->x : -1;
}

/* getY */
int player_getY(player_t* player) {
  return player ? player->y : -1;
}

/* getName */
char* player_getName(player_t* player) {
  return player ? player->playerName : NULL;
}

/* getID */
char player_getID(player_t* player) {
  return player ? player->playerID : '\0';
}
/* getAddress */
addr_t player_getAddress(player_t* player) {
  return player ? player->address : player->address;
}

/* getGold */
int player_getGold(player_t* player) {
  return player ? player->purse : 0;
}
/* getGrid */
grid_t* player_getGrid(player_t* player) {
  return player ? player->visibleGrid : NULL;
}

/* ****************** player_new ****************** */
/* see player.h for detailed documentation */
player_t*
player_new(char playerID, char* playerName, addr_t address, grid_t* masterGrid, int x, int y)
{
  if (playerID == '0') {
    playerName = "SPECTATOR";
  }
  // check for valid x, y
  if (x < 0 || y < 0) {
    return NULL;
  }
  if (x >= grid_getXLength(masterGrid) || y >= grid_getYLength(masterGrid)) {
    return NULL;
  }

  player_t* newPlayer = malloc(sizeof(player_t)); // allocate memory for player
  // check pointer for NULL
  if (newPlayer == NULL) {
    return NULL;
  }
  char* name = malloc(strlen(playerName)+1);
  strcpy(name, playerName);
  newPlayer->playerName = name;
  newPlayer->address = address;
  newPlayer->x = x; // set x location to x
  newPlayer->y = y; // set y location to y
  newPlayer->playerID = playerID; // assign playerID
  newPlayer->purse = 0; // initialize purse to 0
  newPlayer->visibleGrid = visibility_calculatePlayerVisibility(masterGrid, newPlayer); // calculate visibility
  return newPlayer;
}

/* ****************** player_setGrid ****************** */
/* see player.h for detailed documentation */
void
player_setGrid(player_t* player, grid_t* grid)
{
  // check for null pointer
  if (player == NULL) {

  }
  player->visibleGrid = grid;
}



/* ****************** player_updateLocation ****************** */
/* see player.h for detailed documentation */
void
player_updateLocation(player_t* player,  int x, int y)
{
  // check for null parameter
  if (player == NULL) {
    return;
  }
  player->x = x; // set new x
  player->y = y; // set new y
}

/* ****************** player_delete ****************** */
/* see player.h for detailed documentation */
void
player_delete(player_t* player)
{
  // check for null parameter
  if (player == NULL) {
    return;
  }
  free(player_getName(player));
  grid_freeGrid(player->visibleGrid);
  free(player);
}


void
player_addGold(player_t* player, int goldGained)
{
  if (player == NULL) {
    return;
  }
  player->purse = player->purse + goldGained;
}
