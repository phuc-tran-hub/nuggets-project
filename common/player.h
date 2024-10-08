/*
 * player.h -> implementation of player object and related functions
 *
 * Henry Bonnie   Nov. 7th   CS50 F21
 * Stanley Gao '24, CS50 21F
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "grid.h"
#include "message.h"

/* ****************** player ****************** */
/* 
 * player -> struct holding information for a specific player within
 *  the nuggets game. Holds the players ID, their x and y location,
 *  their visible grid, and their gold purse
 */
typedef struct player player_t;


/* ****************** getter functions ****************** */
/* getX */
int player_getX(player_t* player);

/* getY */
int player_getY(player_t* player);
  
/* getName */
char* player_getName(player_t* player);

/* getID */
char player_getID(player_t* player);

/* getGrid */
grid_t* player_getGrid(player_t* player);

/* getAddress */
addr_t player_getAddress(player_t* player);

/* getGold */
int player_getGold(player_t* player);

/* ****************** setter functions ****************** */
/* setGrid */
void player_setGrid(player_t* player, grid_t* grid);

/* ****************** player_new ****************** */
/*
 * player_new -> initializes a new player struct, takes
 *  a playerID and the master grid as parameters. Returns 
 *  a pointer to a player struct if successfully initialized,
 *  otherwise will exit with an error status. Memory will be
 *  allocated for the player so the caller is responsible
 *  for later freeing the memory by calling player_delete();
 * 
 * returns NULL if memory allocation fails, playerID is bad, or x and ys are outside of the grid
 */

player_t* player_new(char playerID, char* playerName, addr_t address, grid_t* masterGrid, int x, int y);

/* ****************** player_getGrid ****************** */
/* 
 * player_getGrid -> getter function that return a pointer to a
 *  players visible grid. Takes that player as a parameter and
 *  returns a pointer to their visible grid
 * 
 * returns NULL if player object is NULL
 */
grid_t* player_getGrid(player_t* player);

/* ****************** player_updateLocation ****************** */
/* 
 * player_updateLocation -> function to update a players location
 *  on the grid. Takes a pointer to a player struct as a parameter
 *  as well as a new X and Y location on the grid.
 * 
 * returns silently and early if player is NULL
 */
void player_updateLocation(player_t* player, int x, int y);

/* ****************** player_delete ****************** */
/* 
 * player_delete -> function to free a player struct from memory. Will
 *  free their visible grid by calling grid_delete() and then will free
 *  the struct from memory. When called correctly there should be no memory
 *  leaks
 * 
 * returns silently and early if player is NULL (does not need to be freed)
 */
void player_delete(player_t* player);

/*
 * player_addGold -> function to increment a players purse
 *  when they step on gold. Takes gold gained as a parameter
 *  and adds it to their purse.
 */
void player_addGold(player_t* player, int goldGained);
