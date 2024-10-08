/*
 * game.c -> see game.h for detailed documentation
 *
 * Henry Bonnie   Nov. 7th   CS50 F21
 */

#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "file.h"
#include "message.h"
#include "grid.h"
#include "gold.h"
#include "player.h"
#include "compress.c"

/* ****************** game constants ****************** */
const int MaxNameLength = 50;   // max number of chars in playerName
const int MaxPlayers = 26;      // maximum number of players
const int GoldTotal = 250;      // amount of gold in the game
const int GoldMinNumPiles = 10; // minimum number of gold piles
const int GoldMaxNumPiles = 30; // maximum number of gold piles

/* ****************** game ****************** */
typedef struct game {
  grid_t* master; // master grid
  player_t** players; // player array
  gold_t** gold; // gold array
  player_t* spectator; // spectator
  int xLen; // x length of map
  int yLen; // y length of map
  int goldRemaining; // gold remaining
  int numPiles; // number of gold piles
} game_t;


/* ****************** getter functions ****************** */
/* getMaster */
grid_t* game_getMaster(game_t* game) {
  return game ? game->master : NULL; }

/* getGold */
gold_t** game_getGold(game_t* game) {
  return game ? game->gold : NULL; }

/* getSpectator */
bool game_getSpectator(game_t* game){
  return game ? game->spectator : false; }

/* getXLen */
int game_getXLen(game_t* game) {
  return game ? game->xLen : -1; }

/* getYLen */
int game_getYLen(game_t* game) {
  return game ? game->yLen : -1; }

/* getSpectator */
int game_getGoldRemaining(game_t* game) {
  return game ? game->goldRemaining : -1; }

/* getNumPiles */
int game_getNumPiles(game_t* game) {
  return game ? game->numPiles : -1; }

/* getPlayers */
player_t** game_getPlayers(game_t* game) {
  return game ? game->players : NULL; }

/* ****************** game_new ****************** */
/* see game.h for detailed documentation */
game_t*
game_new(grid_t* master)
{
  if (master == NULL) {
    return NULL;
  }
  game_t* game = malloc(sizeof(game_t));
  if (game == NULL) {
    return NULL;
  }
  game->master = master;
  player_t** temp;
  temp = calloc(MaxPlayers, sizeof(player_t*));
  game->goldRemaining = GoldTotal;
  for (int i = 0; i< MaxPlayers; i++) {
    temp[i] = NULL;
  }
  game->players = temp;
  game->spectator = NULL;
  return game;
}

/* ****************** game_delete ****************** */
/* see game.h for detailed documentation */
void
game_delete(game_t* game)
{
  if (game == NULL) {
    return;
  }
  grid_freeGrid(game->master);
  if (game_getSpectator(game) == true) {
    free(game->spectator);
  }
  int i = 0;
  while (game->players[i] != NULL) {
    player_delete(game->players[i]);
    i++;
  }
  free(game->players);
  i = 0;
  while	(game->gold[i] != NULL) {
    gold_delete(game->gold[i]);
    i++;
  }
  free(game->gold);
  free(game);
}

/* ****************** game_newPlayer ****************** */
/* see game.h for detailed documentation */ 
char
game_newPlayer(game_t* game, char* playerName, addr_t address)
{
  if (game == NULL) {
    return '0';
  }
  char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i = 0;
  player_t** players = game->players;
  while (i < MaxPlayers) {
    if (players[i] == NULL) {
      int x = rand() % game_getXLen(game);
      int y = rand() % game_getYLen(game);
      while (grid_checkSpot(game_getMaster(game), x, y, '.') == false) {
        x = rand() % game_getXLen(game);
        y = rand() % game_getYLen(game);
      }
      players[i] = player_new(alphabet[i], playerName, address, game_getMaster(game), x, y);
      return alphabet[i];
    }
    i++;
  }
  return '0';
}

/* ****************** game_setDimensions ****************** */
/* see game.h for detailed documentation */
void
game_setDimensions(game_t* game, int nRows, int nCols)
{
  if (game == NULL) {
    return;
  }
  game->xLen = nRows;
  game->yLen = nCols;
}

/* ****************** game_setSpectator ****************** */
/* see game.h for detailed documentation */
bool
game_setSpectator(game_t* game, player_t* player)
{
  if (game == NULL) {
    return false;
  }
  if (game->spectator != NULL) {
    message_send(player_getAddress(game->spectator), "QUIT You have been replaced by new spectator\n");
    player_delete(game->spectator);
  }
  player_setGrid(player, game_getMaster(game));
  game->spectator = player;
  return true;
}

/* ****************** game_findPlayer ****************** */
/* see game.h for detailed documentation */
player_t*
game_findPlayer(game_t* game, addr_t address)
{
  if (game == NULL) {
    return NULL;
  }
  int i = 0;
  player_t** players = game->players;
  while (players[i] != NULL) {
    if (message_eqAddr(player_getAddress(players[i]), address)) {
      return players[i];
    }
    i++;
  }
  return NULL;
}

/* ****************** game_sendAll ****************** */
/* see game.h for detailed documentation */
void 
game_sendAll(game_t* game, void* arg, void (*itemfunc)(game_t* game, void* arg, player_t* player))
{
  if (game == NULL || itemfunc == NULL) {
    return;
  }
  int i = 0;
  player_t** players = game->players;
  while (players[i] != NULL) {
    (*itemfunc)(game, arg, players[i]);
    i++;
  }
  if (game->spectator != NULL) {
    printf("here\n");
    grid_t* compressedGrid =  compress_spectatorCompression(game, game_getGold(game), game_getPlayers(game));
    printf("here1\n");
    player_setGrid(game->spectator, compressedGrid);
    char* display = grid_toString(player_getGrid(game->spectator));
    char* displayMessage = malloc(strlen("DISPLAY") + strlen(display) +2);
    sprintf(displayMessage, "DISPLAY\n%s", display);
    message_send(player_getAddress(game->spectator), displayMessage);
    free(displayMessage);
    free(display);
    printf("here2\n");
  }
}

/* ****************** sendGold ****************** */
/* see game.h for detailed documentation */
void
sendGold(game_t* game, void* arg, player_t* player)
{
  int* goldGained = (int*) arg;
  char* goldMessage = malloc(strlen("GOLD 250 280 280") + 1);
  player_addGold(player, *goldGained);
  sprintf(goldMessage, "GOLD %d %d %d", *goldGained, player_getGold(player), game_getGoldRemaining(game));
  printf("%s\n", goldMessage);
  message_send(player_getAddress(player), goldMessage);
  free(goldMessage);
}

/* ****************** sendDisplay ****************** */
/* see game.h for detailed documentation */
void
sendDisplay(game_t* game, void* arg, player_t* player)
{
  grid_t* newVisible = visibility_calculatePlayerVisibility(game_getMaster(game), player);
  grid_union(player_getGrid(player), newVisible);
  grid_t* compressedGrid =  compress_doCompression(game, player, game_getGold(game), game_getPlayers(game));
  char* display = grid_toString(compressedGrid);
  char* displayMessage = malloc(strlen("DISPLAY") + strlen(display) +2);
  sprintf(displayMessage, "DISPLAY\n%s", display);
  message_send(player_getAddress(player), displayMessage);
  free(displayMessage);
  free(display);
  grid_freeGrid(compressedGrid);
  grid_freeGrid(newVisible);
}

/* ****************** sendScores ****************** */
/* see game.h for detailed documentation */
void
sendScores(game_t* game, void* arg, player_t* player)
{
  char* scores = malloc(MaxPlayers*(strlen(player_getName(player)) + sizeof(char) * 10));
  strcpy(scores, "QUIT GAME OVER:\n");
  player_t** players = game_getPlayers(game);
  player_t* temp;
  for (int i = 0; i<MaxPlayers; i++) {
    if ((temp = players[i]) != NULL) {
      char* score = malloc(strlen(player_getName(players[i])) + sizeof(char) * 10);
      sprintf(score, "%c\t%s %d\n", player_getID(temp), player_getName(temp), player_getGold(temp));
      strcat(scores, score);
      free(score);
    }
  }
  message_send(player_getAddress(player), scores);
  free(scores);
}

/* ****************** game_updateGoldRemaining ****************** */
/* see game.h for detailed documentation */
void
game_updateGoldRemaining(game_t* game, int gold) {
  if (game == NULL) {
    return;
  }
  game->goldRemaining = game_getGoldRemaining(game) - gold;
}

/* ****************** game_spotContainsGold ****************** */
/* see game.h for detailed documentation */
gold_t*
game_spotContainsGold(game_t* game, int x, int y)
{
  int i = 0;
  gold_t** piles = game_getGold(game);
  while (piles[i] != NULL) {
    if (x == gold_getX(piles[i]) && y == gold_getY(piles[i]) && gold_getGold(piles[i]) != 0) {
      return piles[i];
    }
    i++;
  }
  return NULL;
}

/* ****************** game_setGold ****************** */
/* see game.h for detailed documentation */
void
game_setGold(game_t* game, gold_t** gold)
{
 if (game == NULL || gold == NULL) {
    return;
  }
 game->gold = gold;
}


void
game_setNumPiles(game_t* game, int numPiles)
{
  if (game == NULL) {
    return;
  }
  game->numPiles = numPiles;
}

/* ****************** game_switchPlayers ****************** */
/* see game.h for detailed documentation */
void
game_switchPlayers(game_t* game, int x, int y, int oldX, int oldY)
{
  int i = 0;
  player_t** players = game_getPlayers(game);
  while (players[i] != NULL) {
    if (x == player_getX(players[i]) && y == player_getY(players[i])) {
      player_updateLocation(players[i], oldX, oldY);
    }
    i++;
  }
}
