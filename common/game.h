/*
 * game.h -> implementation of game module for nuggets game
 *
 * Henry Bonnie   Nov. 11th   CS50 F21
 * Stanley Gao '24, CS50, 21F
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "file.h"
#include "message.h"
#include "grid.h"
#include "gold.h"
#include "player.h"

/* ****************** game constants ****************** */
const int MaxNameLength;
const int MaxPlayers;
const int GoldTotal;
const int GoldMinNumPiles;
const int GoldMaxNumPiles;

/* ****************** game ****************** */
typedef struct game game_t;

/* ****************** getter functions ****************** */
grid_t* game_getMaster(game_t* game);
gold_t** game_getGold(game_t* game);
bool game_getSpectator(game_t* game);
int game_getXLen(game_t* game);
int game_getYLen(game_t* game);
int game_getGoldRemaining(game_t* game);
int game_getNumPiles(game_t* game);
player_t** game_getPlayers(game_t* game);

/* ****************** setter functions ****************** */
void game_setDimensions(game_t* game, int nRows, int nCols);
bool game_setSpectator(game_t* game, player_t* player);
void game_setGold(game_t* game, gold_t** gold);

/* ****************** game_new ****************** */
/*
 * game_new -> 
 *
 * returns NULL if null grid provided, memory alloc errors
 */
game_t* game_new(grid_t* master);

/* ****************** game_delete ****************** */
/* 
 * game_delete -> 
 *
 * returns silently and early if game provided is NULL (no delete needed)
 */
void game_delete(game_t* game);

/* ****************** game_newPlayer ****************** */
/*
 * game_newPlayer -> 
 *
 * returns char '0' if game is NULL, or other error
 */
char game_newPlayer(game_t* game, char* playerName, addr_t addressFrom);

/* ****************** game_findPlayer ****************** */
/* 
 * game_findPlayer ->
 *
 * returns NULL if game is NULL
 */
player_t* game_findPlayer(game_t*, addr_t address);

/* ****************** game_sendAll ****************** */
/*
 * game_sendAll ->
 *
 */
void game_sendAll(game_t* game, void* arg, void (*itemfunc)(game_t* game, void* arg, player_t* player));

/* ****************** game_updateGoldRemaining ****************** */
/* 
 * game_updateGoldRemaining ->
 *
 */
void game_updateGoldRemaining(game_t* game, int gold);

/* ****************** game_spotContainsGold ****************** */
/*
 * game_spotContainsGold ->
 *
 */
gold_t* game_spotContainsGold(game_t* game, int x, int y);

/* ****************** sendGold ****************** */
/*
 * sendGold ->
 *
 * returns NULL if game or gold array is NULL
 */
void sendGold(game_t* game, void* arg, player_t* player);

/* ****************** sendDisplay ****************** */
/*
 * sendDisplay ->
 *
 */
void sendDisplay(game_t* game, void* arg, player_t* player);


void sendScores(game_t* game, void* arg, player_t* player);

void game_setNumPiles(game_t* game, int numPiles);

void game_switchPlayers(game_t* game, int x, int y, int oldX, int oldY);
