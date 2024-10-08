/* 
 * client.c - a client for the nuggets game, representing a player or a spectator
 * 
 * Phuc Tran - November 10th, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#include "support/message.h"

/**************** local types ****************/
typedef struct localclient {
  // player's characteristics
  char playerID[2];
  int purse;
  int goldRemaining;

  // the size of the window
  char nRows[5];
  char nCols[5];

} localclient_t;


localclient_t currentClient;

/**************** FUNCTION ****************/

/**************** file-local functions ****************/
static int parseArgs(const int argc, char* argv[]);

// handling communications
static int playGame(const addr_t serverAddress);
static bool handleInput  (void* arg);
static bool handleMessages(void* arg, const addr_t from, const char* message);

// handling specific communication
static void handleOk(const char* message);
static void handleGrid(const char* message);
static void handleGold(const char* message);
static void handleDisplay(const char* message);
static bool handleQuit(const char* message);
static void handleError(const char* message);


// handling ncurse displays
static void handleSize(char* nRowsStr, char* nColsStr);
static void setupWindow(void);
static void printDisplay(const char* displayString);

/**************** main ****************/
/* see client.h for description */
int main(const int argc, char* argv[]){

  int ok = 0;
  // validating arguments
  int checkArgs; 
  if ((checkArgs = parseArgs(argc, argv)) != 0){
    exit(checkArgs);
  } else{
    const char* serverHost = argv[1];
    const char* serverPort = argv[2];
    addr_t server;
    message_setAddr(serverHost, serverPort, &server);

    ok = playGame(server);
  }
  
  return ok; // status code depends on result of message_loop
}


/**************** parseArgs ****************/
/* see client.h for description */
static int parseArgs(const int argc, char* argv[]){

  // validate command lines
  if (message_init(NULL) == 0) {
    return 2; // failure to initialize message module
  }

  // check arguments
  const char* program = argv[0];
  if (argc < 1 || argc > 4) {
    fprintf(stderr, "usage: %s hostname port\n", program);
    fprintf(stderr, "incorrect number of commandline arguments %d \n", argc);
    return 3; // bad commandline
  }

  // get the serverAddress from serverHost and serverPost arguments
  const char* serverHost = argv[1];
  const char* serverPort = argv[2];
  addr_t server; // address of the server
  if (!message_setAddr(serverHost, serverPort, &server)) {
    fprintf(stderr, "can't form address from %s %s\n", serverHost, serverPort);
    return 4; // bad hostname/port
  }

  // send "PLAY [playername] for player client"
  if (argc == 4){
    char* playerName = argv[3];

    // mallocing for play message
    char* message = (char *) malloc(7 + strlen(playerName) + 1);
    strcpy(message, "PLAY ");
    strcat(message, playerName);

    // setting up ncurses window, sending first communication message
    setupWindow();
    fprintf(stderr, "interactive player mode; name '%s' \n", playerName);
    message_send(server, message);
    free(message);
  }

  // send "PLAY [playername] for player client"
  else if (argc == 3){

    // mallocing for spectate message
    char* message = (char *) malloc(10);
    strcpy(message, "SPECTATE");

    // setting up ncurses window, sending first communication message
    setupWindow();
    message_send(server, message);
    free(message);
  }

  // return 0 for no fatal commandline arguments
  return 0;
}


/**************** playGame ****************/
/* see client.h for description */
static int playGame(addr_t server){

  bool ok = message_loop(&server, 0, NULL, handleInput, handleMessages);

  // shuts down the module when done
  message_done();

  // status code depends on result of message_loop
  return ok? 0 : 1;
}

/**************** handleInput ****************/
/* see client.h for description */
static bool handleInput(void* arg){
  // We use 'arg' to receive an addr_t referring to the 'server' correspondent.
  // Defensive checks ensure it is not NULL pointer or non-address value.
  addr_t* serverp = arg;
  if (serverp == NULL) {
    fprintf(stderr, "handleInput called with arg=NULL");
    return true;
  }
  if (!message_isAddr(*serverp)) {
    fprintf(stderr, "handleInput called without a correspondent.");
    return true;
  }

  // continuous loop for a window char input
  char c;
  if ((c = getch()) != 'q') {    // read one character
    // package input and send to server
    char* packagedMessage = (char *) malloc(6);
    sprintf(packagedMessage, "KEY %c", c);
    message_send(*serverp, packagedMessage);
    free(packagedMessage);
    
    // normal case: keep looping
    return false;
  } else{

    // if the client presses q, exits preemptively
    endwin();
		return true;
	}
}


/**************** handleMessage ****************/
/* see client.h for description */
static bool handleMessages(void* arg, const addr_t from, const char* message){

  // check message for NULL
  if (message == NULL){
    fprintf(stderr, "ERROR message is null \n");
    return false;
  }

  // handling OK message
  if (strncmp(message, "OK ", strlen("OK ")) == 0){
    handleOk(message);
  }

  // handling GRID message
  else if (strncmp(message, "GRID ", strlen("GRID ")) == 0){
    handleGrid(message);
  }

  // handling GOLD message
  else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0){
    handleGold(message);
  }

  // handling DISPLAY message
  else if (strncmp(message, "DISPLAY" , strlen("DISPLAY")) == 0){
    handleDisplay(message);
  }

  // handling QUIT message
  else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0){
    return handleQuit(message);
  }

  // handling ERROR message
  else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0){
    handleError(message);
  }

  return false;
}


/**************** handleOk ****************/
/* see client.h for description */
static void handleOk(const char* message){

  // parsing out charID and setting that to the current client's ID
  char charID[2];
  sscanf(message, "OK %s", charID);
  fprintf(stderr, "%s \n", charID);
  // defensive coding for valid message
  if (charID != NULL){
    fprintf(stderr, "OK message received: OK %s \n", charID);
    sprintf(currentClient.playerID, "%s", charID);
  }
}


/**************** handleGrid ****************/
/* see client.h for description */
static void handleGrid(const char* message){

    // parsing out nRows and nCols and setting those to the current client's window's size
    char nRowsStr[5];
    char nColsStr[5];
    sscanf(message, "GRID %s %s", nRowsStr, nColsStr);

    // saving it onto nRows and nCols of current player
    sprintf(currentClient.nRows, "%s", nRowsStr);
    sprintf(currentClient.nCols, "%s", nColsStr);

    // defensive coding for valid message
    if (nRowsStr != NULL && nColsStr != NULL){

      // adjust window's size to fit map
      fprintf(stderr, "GRID message received: GRID %s %s \n", nRowsStr, nColsStr);
      handleSize(nRowsStr, nColsStr);
    }
}


/**************** handleGold ****************/
/* see client.h for description */
static void handleGold(const char* message){
  // parsing out n, p, and r parameters
  int n;
  int p;
  int r;
  sscanf(message, "GOLD %d %d %d", &n, &p, &r);

  // defensive coding to check valid message
  if (n >= 0 && p >= 0 && r >= 0){
    fprintf(stderr, "GOLD message received: GOLD %d %d %d \n", n, p, r);
    currentClient.purse = p;
    currentClient.goldRemaining = r;

    attron(COLOR_PAIR(1));
    if (currentClient.playerID != NULL){
      // writing the client's ID, purse, and gold remaining accordingly
      if (n == 0){
        move(0, 0);
        printw("Player %s has %d nuggets (%d nuggets unclaimed). \n", currentClient.playerID, currentClient.purse, currentClient.goldRemaining);
        refresh();
      } else{ 
        move(0, 0);
        printw("Player %s has %d nuggets (%d nuggets unclaimed). GOLD received %d\n", currentClient.playerID, currentClient.purse, currentClient.goldRemaining, n);
        refresh();
      }
    }
    attroff(COLOR_PAIR(1)); // turn off the screen color
  }
}


/**************** handleDisplay ****************/
/* see client.h for description */
static void handleDisplay(const char* message){
  fprintf(stderr, "DISPLAY: \n");
  // wrapper to printDisplay
  printDisplay(message);
}


/**************** handleQuit ****************/
/* see client.h for description */
static bool handleQuit(const char* message){

  // printing the QUIT message's explanation, erasing the window, cleaning the memory, and ending the window
  fprintf(stderr, "QUIT message received:");
  fprintf(stderr, "%s \n", message);
  erase();
  endwin(); // turn off curses display
  fprintf(stdout, "%s \n", message);
  return true;
}


/**************** handleError ****************/
/* see client.h for description */
static void handleError(const char* message){
  fprintf(stderr, "%s \n", message);
}


/**************** setupWindow ****************/
/* see client.h for description */
static void setupWindow(){

  // initialize a screen
  newterm(NULL, stdout, stdin);
  cbreak();  // accept keystrokes immediately
  noecho();  // don't echo characters to the screen
  start_color(); // turn on color controls
  init_pair(1, COLOR_YELLOW, COLOR_BLACK); // define color 1

	refresh(); // redraw the screen
}

/**************** handleSize ****************/
/* see client.h for description */
static void handleSize(char* nRowsStr, char* nColsStr){
  // finding the current window's size and the intended grid's size
  int currentRow;
  int currentCol;
  getmaxyx(stdscr, currentRow, currentCol);

  // converting to int to compare values
  int nRows = atoi(nRowsStr) + 2;
  int nCols = atoi(nColsStr) + 2 ;

  // continue asking to resize window's size
  while(currentRow < nRows + 1 || currentCol < nCols + 1){
    printw("Your window must be at least %d high \n", nRows);
    printw("Your window must be at least %d wide \n", nCols);
    printw("Resize your window, and press Space to continue. \n");

    if (getch() == ' ') {
      getmaxyx(stdscr, currentRow, currentCol);
    }
    
    clear();
    refresh();
  }

  // sending a confirmation log
  fprintf(stderr, "window's size now fits. \n");
}

/**************** printDisplay ****************/
/* see client.h for description */
static void printDisplay(const char* displayString){

  // initializing cursor
  int y = 1;
  int x = 0;
  move(y, x);
  int currentIdx = 0;

  // size of grid plus all the \n on each row and the \0
  int size = atoi(currentClient.nRows) * atoi(currentClient.nCols) + atoi(currentClient.nRows) + 1;
  char display[size];

  int messageIdx = 8;
  // iteraing through the message and the parsed display
  for(int displayIdx = 0; displayIdx < strlen(displayString); displayIdx++){
    display[displayIdx] = displayString[messageIdx];
    messageIdx++;
  }

  // printing every character of the display to window
  while(display[currentIdx] != '\0'){
    addch(display[currentIdx]);
    currentIdx++;
  }

  refresh();
}
