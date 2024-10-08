/* 
 * client.h - a header file for 'client' module, explaining all functions
 * 
 * Phuc Tran - November 2021
 */

#include "support/message.h"

/**************** local types ****************/
typedef struct localplayer {
  // player's characteristics
  char* playerID;
  int purse;
  int goldRemaining;

  // the size of the window
  char* nRows;
  char* nCols;

} localplayer_t;


localplayer_t currentPlayer;

/**************** FUNCTION ****************/

/**************** file-local functions ****************/
static int parseArgs(const int argc, char* argv[]);

// handling communications
static int playGame(const addr_t serverAddress);
static bool handleInput  (void* arg);
static bool handleMessages(void* arg, const addr_t from, const char* message);

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

static void cleanPlayer(void);
/***************** main *******************************/
/* calls parseArgs and playGame.
 * Return nothing.
 */
int main(const int argc, char* argv[]);

/**************** parseArgs ****************/
/* takes the command line arguments as parameters, validates them, and 
 * making connection to server according to the command line arguments (player vs spectator)
 * Return an integer to identify arguments error.
 */
static int parseArgs(const int argc, char* argv[]);

/**************** playGame ****************/
/* acts as a wrapper to call message_loop
 * Return an integer to identify game error.
*/
static int playGame(addr_t server);

/**************** handleInput ****************/
/* takes the key input from stdin and combine it with a "KEY " to send to server
 * 'arg' is the server's address.
 * Return true if the message loop should exit, otherwise false.
 */
static bool handleInput(void* arg);

/**************** handleMessage ****************/
/* takes a message from the server, parses it accordingly, validate them, and handle specific to the command type.
 * We ignore 'arg' here.
 * Return true if the message loop should exit, otherwise false.
 */
static bool handleMessages(void* arg, const addr_t from, const char* message);

/**************** handleOk ****************/
/* handles an OK message according to specs.
*/
static void handleOk(const char* message);

/**************** handleGrid ****************/
/* handles an GRID message according to specs.
*/
static void handleGrid(const char* message);

/**************** handleGrid ****************/
/* handles an GOLD message according to specs.
*/
static void handleGold(const char* message);

/**************** handleDisplay ****************/
/* handles an DISPLAY message according to specs.
 */
static void handleDisplay(const char* message);

/**************** handleQuit ****************/
/* handles an QUIT message according to specs.
*/
static bool handleQuit(const char* message);

/**************** handleError ****************/
/* handles an ERROR message according to specs.
*/
static void handleError(const char* message);

/**************** setupWindow ****************/
/* sets up a game window using the ncurses library.
*/
static void setupWindow();

/**************** handleSize ****************/
/* continuously asks the client to resize window until it fits the map.
 */
static void handleSize(char* nRowsStr, char* nColsStr);

/**************** printDisplay ****************/
/* prints the display to the ncurses window according to the message.
 */
static void printDisplay(const char* displayString);

/**************** cleanPlayer ****************/
/* clean the global player struct and the reachable bytes.
 */
static void cleanPlayer();