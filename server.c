
/*
 * server.c -> implementation of server
 *  This module is the server that for the nuggets game and does
 *  holds all information necessary for the game to message with client
 *  and conduct the game.
 *
 * usage: ./server 
 *  [mapFile] [optional: seed]
 *
 * Henry Bonnie   Nov. 7th   CS50 F21
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "message.h"
#include "grid.h"
#include "file.h"
#include "gold.h"
#include "player.h"
#include "game.h"
#include "visibility.h"
#include "compress.h"

/* ***************** local functions ****************** */
static void parseArgs(const int argc, char* argv[], char** map, int* seed);
static gold_t** initializeGold(grid_t* baseMap);
static void playGame(void);
static bool handleMessages(void* arg, const addr_t addressFrom, const char* message);
static void checkLocation(player_t* player, int x, int y);

/* ***************** handlers ***************** */
static void handlePlay(const addr_t addressFrom, char* command);
static void handleKey(const addr_t addressFrom, const char* message, player_t* player);
static void handleSpectate(const addr_t addressFrom);

/* ****************** global variable ****************** */
game_t* game;

/* ****************** main ****************** */
int
main(int argc, char* argv[])
{
  int seed; // seed for srand
  char** mapFile = malloc(sizeof(char*)); // file containing map
  parseArgs(argc, argv, mapFile, &seed); // parse arguments
  game = game_new(grid_loadGridFromFile(*mapFile)); // initialize game with map as master
  game_setDimensions(game, grid_getXLength(game_getMaster(game)), grid_getYLength(game_getMaster(game))); // set dimensions
  game_setGold(game, initializeGold(game_getMaster(game))); // set game gold to initialized gold
  playGame(); // game runner
  game_delete(game); // delete game
  free(mapFile); // free map file from memory
}


/* ****************** parseArgs ****************** */
static void
parseArgs(const int argc, char* argv[], char** map, int* seed)
{
  FILE* fp;
  // check correct number of arguments
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Error: Incorrect Number of Arguments\n");
    exit(1);
  }
  // attempt to open mapfile
  if ((fp = fopen(argv[1], "r")) != NULL) {
    // reallocate memory's size to argument's length
    char* p = (char *)realloc(*map, strlen(argv[1]) + 1);
    *map = p;
    *map = argv[1];

    free(p);
  } else {
    fprintf(stderr, "Error: Could not open map file\n");
    exit(1);
  }
  // check for seed
  if (argc == 3) {
    sscanf(argv[2], "%d", seed); // scan for seed
    srand(*seed); // call srand
  } else {
    srand(getpid()); // if no seed use getpid
  }
  fclose(fp);
}

/* ****************** initializeGold ****************** */
/*
 * initializeGold -> function that initializes an array of pointers
 *  to gold_t objects with random x and y locations within the map as well
 *  as a random amount of gold. Takes the master grid as a parameter
 */
gold_t**
initializeGold(grid_t* baseMap)
{
  int numPiles = rand() % GoldMaxNumPiles; // random num piles
  // make sure numPiles > min
  while(numPiles < GoldMinNumPiles) {
    numPiles = rand() % GoldMaxNumPiles;
  }
  game_setNumPiles(game, numPiles);
  gold_t** gold	= (gold_t**)calloc(numPiles + 1, sizeof(gold_t*)); // initialize memory for array of gold objects
  // iterate piles
  for (int i = 0; i < numPiles; i++) {
    gold[i] = NULL;
    int x = -1;
    int y = -1;
    while (grid_checkSpot(game_getMaster(game), x, y, '.') == false) {
      x = rand() % game_getXLen(game); // get random x location within boundary
      y = rand() % game_getYLen(game); // get random y location within boundary
      // iterate objects already created
      for (int j = 0; j <=i; j++) {
        // if there is already a gold spot on x an y
        if (x == gold_getX(gold[j]) && y == gold_getY(gold[j])) { 
          y = -1; // reset loop
          x = -1; // reset loop
        }
      }
    }
    gold[i] = gold_new(0, x, y); // create new gold object with 0 gold
  }
  int goldTemp = game_getGoldRemaining(game); // temp value to distribute gold
  // iterate until all gold assigned
  while (goldTemp != 0) {
    // iterate piles
    for (int j = 0; j < numPiles; j++) {
      // 33% probability of assigning nugget
      if (rand() % 3 == 0) {
        gold_setGold(gold[j], gold_getGold(gold[j]) + 1); // increment gold by 1
        goldTemp--; // decrease gold temp
        // break for loop
        if (goldTemp == 0) {
          j = 1000; // break loop
        }
      }
    }
  }
  return gold;
}

/* ****************** playGame ****************** */
/* 
 * play_game -> runner for the game. Initializes the message
 *  look until termination.
 */
static void
playGame(void)
{ 
  int ourPort = message_init(stderr);
  fprintf(stderr, "serverPort=%d", ourPort);
  message_loop(NULL, 0, NULL, NULL, handleMessages);                                      
  message_done();
}
  
/* ****************** handleMessage ****************** */
/*
 * handleMessage -> handleMessage function for the message loop.
 *  parses identifying message from loop and calls corresponding handle
 *  or prints error message if identifier is unknown. Returns true
 *  when gold is NULL
 *
 */
static bool
handleMessages(void* arg, const addr_t addressFrom, const char* message)
{
  char* identifier = malloc(sizeof(char)*10); // malloc memory for identifier
  char* command = malloc(strlen(message)+1); // malloc memory for command
  sscanf(message, "%s %s", identifier, command); // scan into pointers
  // if identifier is play
  if (strcmp(identifier, "PLAY") == 0) {
    handlePlay(addressFrom, command); // call handlePlay
    // if identifier is spectate
  } else if (strcmp(identifier, "SPECTATE") == 0) {
    handleSpectate(addressFrom); // handleSpectate
    // if identifier is key
  } else if (strcmp(identifier, "KEY") == 0) {
    player_t* player = game_findPlayer(game, addressFrom); // find which player to move
    handleKey(addressFrom, message, player); // call handleKey
  } else {
    message_send(addressFrom, "ERROR Message doesn't match expected format");
  }
  free(identifier); // free memory
  free(command); // free memory
  if (game_getGoldRemaining(game) == 0) {
    game_sendAll(game, NULL, sendScores); // send scores to all                                                        
    return true;
  }
  return false;
}
/* ****************** handlePlay ****************** */
/*
 * handlePlay-> function to handle message when idenifying word
 *  was "PLAY". Sends OK [playerID] and then the grid specs.
 */
static void
handlePlay(const addr_t addressFrom, char* command)
{
  char newPlayer = game_newPlayer(game, command, addressFrom); // initialize new player
  // check for NULL return
  if (newPlayer != '\0') {
    char* response = malloc(strlen("OK A") +1); // malloc for size of message
    sprintf(response, "OK %c", newPlayer); // put player ID into message
    message_send(addressFrom, response); // send OK message
    free(response); // free memory
    response = malloc(strlen("GRID 100 100") + 1); // allocate memory for grid message
    sprintf(response, "GRID %d %d", game_getYLen(game)+1, game_getXLen(game)+1); // scan specs into message
    message_send(addressFrom, response); // send to client
    free(response); // free memory
    int gold = 0;
    game_sendAll(game, &gold, sendGold); // send all
    game_sendAll(game, NULL, sendDisplay);
  }
}

/* ****************** handleSpectate ****************** */
/* 
 * handleSpectate -> function to handle message when the idenfifier
 *  is SPECTATE. Initializes new spectator and removes old one. 
 */
static void
handleSpectate(const addr_t addressFrom)
{
  game_setSpectator(game, player_new('0', NULL, addressFrom,  game_getMaster(game), 0, 0)); // set Spectator
  char* response = malloc(strlen("OK A") +1); // malloc memory for response
  sprintf(response, "OK %c", '0'); // send ok 0
  message_send(addressFrom, response); // send message
  free(response); // free memory
  response = malloc(strlen("GRID 100 100") + 1); // allocate memory to send grid
  sprintf(response, "GRID %d %d", game_getYLen(game)+1, game_getXLen(game)+1);
  message_send(addressFrom, response); // send grid w specs
  free(response); // free memory
  int gold = 0;
  game_sendAll(game, &gold, sendGold); // send gold to all
  game_sendAll(game, NULL, sendDisplay); // send all display
}

/* ****************** handleKey ****************** */
/*
 * handleKey -> function to handle key presses from user. Uses
 *  switch loop to set movement to corresponding move. Checks move
 *  for availability and moves player / picks up available gold on spot
 */
static void
handleKey(const addr_t addressFrom, const char* message, player_t* player)
{
  char keyPress; // variable to hold char keyPress
  char* temp = malloc(sizeof(char)*4); // temp to ignore key
  sscanf(message, "%s %c", temp, &keyPress); // scan message
  free(temp); // free temp variable
  int x = player_getX(player); // get player X
  int y = player_getY(player); // get player Y
  // switch loop for keypress
  switch (keyPress)
    {
    case 'q':
      if (player_getID(player) == '\0') {
        message_send(addressFrom, "QUIT Thanks for watching!\n");
      } else {
        message_send(addressFrom, "QUIT Thanks for playing!\n");
      }
      player_delete(player);
      break;
    case 'h':
      x--;
      break;
    case 'l':
      x++;
      break;
    case 'j':
      y++;
      break;
    case 'k':
      y--;
      break;
    case 'y':
      y--;
      x--;
      break;
    case 'u':
      y--;
      x++;
      break;
    case 'b':
      y++;
      x--;
      break;
    case 'n':
      y++;
      x++;
      break;
    case 'H':
      // continue moving if you can
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y); // update location
        checkLocation(player, x, y); // check for gold
        x--;
      }
      x++;
      break;
    case 'L':
      // continue moving if you can 
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y); // update location
        checkLocation(player, x, y); // check for gold
        x++;
      }
      x--;
      break;
    case 'J':
      // continue moving if you can 
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y); // update location
        checkLocation(player, x, y); // check for gold
        y++;
      }
      y--;
      break;
    case 'K':
      // continue moving if you can 
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y); // update location
        checkLocation(player, x, y); // check for gold
        y--;
      }
      y++;
      break;
    case 'Y':
      // continue moving if you can 
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y); // update location
        checkLocation(player, x, y); // check for gold
        y--;
        x--;
      }
       y++;
       x++;
      break;
    case 'U':
      // continue moving if you can 
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y); // update location
        checkLocation(player, x, y); // check for gold
        y--;
        x++;
      }
       y++;
       x--;
      break;
    case 'B':
      // continue moving if you can 
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y);
        checkLocation(player, x, y);
        y++;
        x--;
      }
      y--;
      x++;
      break;
    case 'N':
      while (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#')) {
        player_updateLocation(player, x, y); // update location
        checkLocation(player, x, y); // check for gold
        y++;
        x++;
      }
      y--;
      x--;
      break;
    }
  if (keyPress != 'q' && (grid_checkSpot(game_getMaster(game), x, y, '.') || grid_checkSpot(game_getMaster(game), x, y, '#'))) {
    game_switchPlayers(game, x, y, player_getX(player), player_getY(player)); // switch players if necessary
    player_updateLocation(player, x, y); // update player location
    checkLocation(player, x, y); // check location for gold
  }
}

/*
 * checkLocation -> function to check for gold on a specific
 *  spot and adds gold to players purse. Sends whole display
 *  after.
 */
static void
checkLocation(player_t* player, int x, int y)
{
  gold_t* gold;
  // check if there is gold on spot
  if ((gold = game_spotContainsGold(game, x, y)) != NULL) {
    int collected = gold_getGold(gold); // get gold from object
    game_updateGoldRemaining(game, collected); // update remaining gold
    int i = 0;
    game_sendAll(game, &i, sendGold); // send to all with new remaining
    sendGold(game, &collected, player); // send gold to individual who collected
    gold_setGold(gold, 0); // set objects gold to 0
  }
  game_sendAll(game, NULL, sendDisplay); // send display to all
}
