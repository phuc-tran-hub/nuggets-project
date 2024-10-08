# CS50 Nuggets

## Implementation Spec

### take-it-to-emacs, Fall, 2021

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.

Our design also includes a `grid` module.

We describe each program and module separately.

We do not describe the `support` library nor the modules that enable features that go beyond the spec.

We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

* grid: Stanley Gao, Ethan Thomas
	* Testing: Henry Bonnie, Phuc Tran
* visibility: Stanley Gao, Ethan Thomas
	* Testing: Henry Bonnie, Phuc Tran
* server: Henry Bonnie, Phuc Tran
	* Testing: Stanley Gao, Ethan Thomas
* Client: Henry Bonnie, Phuc Tran
	* Testing: Ethan Thomas, Stanley Gao

## Client

### Data structures

#### player

`player` is a struct that holds the important client side information for the player. The client is dumb so only need to hold a few items of information such as their playerID and purse.

```c
typedef struct player {
	char playerID;
	int purse;
	addr_t* serverFrom;
} player_t;
```

### Definition of function prototypes

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

The `main` function calls `parseArgs` and other functions.

```c
int main(int argc, char* argv[], char** hostname, int* port, char* playerName)
```

`parseArgs` takes the command line arguments as parameters, validates them, and assigns them to be passed through pointers if valid.

```c
static int parseArgs(const int argc, char* argv[]);
```

sets up a game window using the ncurse library with a given row and column size.

```c
static bool setupWindow(void);
```

`playGame` is essentially the main driver of the client. It runs the message loop until the game is quit and will continuously parse received messages and send messages through the loop. It will also call print display to reprint the display.

```c
static void playGame(const char* serverHost, const char* serverPort, const char*serverAddress);
```

`handleMessage` parses the incoming message to interpret the type of message and displays information onto the game window.

```c
static bool handleMessages(void* arg, addr_t* addressFrom, char* message);
```

`handleInput` waits for keyboard input from the user and sends a corresponding message to the server.

```c
static bool handleInput(void* arg);
```

`printDisplay` iterates through the number of rows and columns, moving the cursor and adding each character of the board onto the screen.

```c
static void printDisplay(char* map);
```

### Detailed pseudo code

#### `main`:

```
	call parseArgs
	call playGame
```

#### `parseArgs`:

```
	validate command line arguments
	initialize message module
	get serverAddress

	print assigned port number
	if there are 4 arguments:
		send PLAY [playername]
	if there are 3 arguments
		Send SPECTATE
	else:
		print error message for incorrect number of arguments
		exit
```

#### `playGame`:

```
	while player hasn’t quit game:
		call messageLoop with NULL for handleTimeout, handleInput, and handleMessage
```

#### `handleMessage`:

```
	check message for NULL
	parses the message
	if first word of message == OK:
		initialize player struct with playerID
	if first word of message == GRID:
		initialize size of grid
		call setupWindow
	if first word of message == GOLD:
		if client is player:
			update players gold value
			update display message
		if client is spectator:
			update how much gold is left
	if first word of message == DISPLAY:
		call printDisplay to load display and update from message
	if first word of message == QUIT:
		end loop
	if first word of message == ERROR:
		print error message
```

#### `setupWindow`:

```
	initialize ncurses window (call initscr())
	set window to nRows nColumns
	call cbreak
```

#### `printDisplay`:

```
	for rows in grid
		for columns in grid
			move cursor to xy
			print next character from string
```

## Server

### Data structures

#### game

`game` is the global struct containing important information about the game. It will hold the master grid, lists of player and gold objects, and a grid of player objects

```c
typedef struct game {
	grid_t* master;
	player_t* players[26];
	gold_t** gold;
	grid_t* players;
} game_t;
```

#### player

A player object represents a player within the game. It holds their personal player id, x and y location on the grid, gold purse, and a grid object for their visible grid that will be passed to that specific player client.

```c
typedef struct player {
	char playerID;
	int x;
	int y;
	int purse;
	grid_t* visibleGrid;
} player_t
```

#### gold

```c
typedef struct gold {
	int x;
	int y;
	int goldStored;
} gold_t;
```

#### grid

Described below.

### Definition of function prototypes

The `main` function calls `parseArgs` and other functions.

```c
int main(int argc, char* argv[])
```

`parseArgs` validates command line arguments.

```c
static int parseArgs(const int argc, char* argv[], char** map, int* map);
```

`initializeGold` creates a `grid` struct of `gold` objects.

```c
static gold_t** initializeGold(grid_t* baseMap, int minNumPiles, int maxNumPiles, int seed);
```

`loadMap` reads mapFile and parses the characters into a single character string. Then, it calls a grid module function to create and assign that grid as the global `game` masterGrid.

```c
static grid_t* loadMap(char* mapFile);
```

`playGame` is essentially the main driver of the server. It runs the message loop until the game is quit and will continuously parse received messages and send messages through the loop.

```c
static void playGame();
```

`handleMessage` parses the incoming message to interpret the type of message, displays information onto the game window, and sends multiple messages to each client.

```c

static void handleMessages(void* arg, const addr_t* addressFrom, char* message);

```

### Detailed pseudo code

#### `main`:

```
	call parseArgs
	call playGame
```

#### parseArgs:

```
	validate command line arguments
	verify map file can be opened for reading
	call loadMap to create the base map for the masterGrid
	if seed provided
		verify it is a valid seed number
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()
		
	call initializeGold to create an array of `gold` objects
	call playGame
```

#### initializeGold

```
	generate random number between minNumPiles and maxNumPiles
	initialize array to size of randomly generated number above
	for item in array
		initialize gold object with random gold value
		assign each gold object’s position to a randomized valid location (a . spot)
```

#### loadMap

```
	parse map.txt into single character string
	pass to gridModule
	assign grid to global `game` masterGrid
```

#### `playGame`:

```
	while there is gold remaining:
		call messageLoop with NULL handleTimeout, NULL handleInput, and handleMessage
```

#### handleMessage

```
	if first word == PLAY:
		if there is space for more players:
			initialize new player object
			save the player object into an array of `player` objects
			respond OK [playerID]
	if first word == SPECTATE:
		if there is a spectator:
			send current spectator QUIT message
			assign client to new spectator
		else:
			assign client as spectator
	if first word == KEY:
		if valid keypress:
			if player can move on map:
				update player X and Y positions
				update the gold grid, player grid, and visibility
				send a unique display to each client according to their visbility
			else:
				send error message
		else:
			Send error message
```

---

## Grid module

This module contains the grid structure and related functions

### Data structures

The `grid` data structure contains information needed to create and run a 2D array – used in this game as the playable area (also known as the map).

```c
typedef struct grid {
	int numRows;
	int numCols;
	char** data;  // represents the map of entities
} grid_t;
```

### Definition of function prototypes

```c
static grid_t* grid_makeEmptyGrid(int numRows, int numCols);
static void grid_freeGrid(grid_t* grid);
static grid_t* grid_loadMapGrid(char* mapfile);

static grid_t* grid_loadGoldGrid(gold_t** goldArray);
static grid_t* grid_loadPlayerGrid(player_t** playerArray);
static void grid_updateGoldGrid(grid_t* goldGrid, gold_t** goldArray);
static void grid_updatePlayerGrid(grid_t* playerGrid, player_t** playerArray);

static bool grid_isRoomSpot(grid_t* mapGrid, int x, int y);


static bool grid_isDirectlyVisible(grid_t* mapGrid, int x1, int y1, int x2, int y2);
static grid_t* grid_calculatePlayerVisibility(grid_t* mapGrid, player_t* player);
static void grid_union(grid_t* grid1, grid_t* grid2);

static void grid_compressLayers(grid_t* compressedGrid, grid_t* mapGrid, gold_t** goldArray, player_t** playerArray);

```

### Detailed pseudo code

#### `grid_makeEmptyGrid`

```
	allocate grid (simple, maybe only one malloc call)
	// also need to be given the size of the map in order to initialize the 2d array
```

#### `grid_freeGrid`

```
	free the grid (simple, maybe only one free call)
```

#### `grid_loadMapGrid`

```
	make an empty grid
	open the file to read
	for each line,
		for each character,
			store the character in the created grid
	return the grid
```

#### `grid_loadGoldGrid`

```
	create an empty grid
	for each gold object in the provided list:
		place than asterisk (*) into the grid at spot (x,y)
	return the grid
```

#### `grid_loadPlayerGrid`

```
	create an empty grid
	for each player object in the provided list:
		place the player’s ID into the grid at spot (x,y)
	return the grid
```

#### `grid_updateGoldGrid`

```
	zero out the gold grid (set all chars to null)
	for each gold object in the provided list:
		place than asterisk (*) into the grid at spot (x,y)
	// no return, as update modifies the provided grid, does not make a new one
```

#### `grid_updatePlayerGrid`

```
	zero out the player grid (set all chars to null)
	for each player object in the provided list:
		place the player’s ID into the grid at spot (x,y)
	// no return, as update modifies the provided grid, does not make a new one
```

#### `grid_isValidPlayerSpot`

```
	get player x, y
	if player x, y in valid spot of mapGrid:
		return true
	return false
```

#### `grid_isDirectlyVisible`

```
	using the map grid, calculate if x1,y1 is directly visible to x2,y2 by looking at the spots around x1,y1
```

#### `grid_calculatePlayerVisibility`

```
	get player's coordinates (pr,pc)
	for each point (r,c) in the grid:
		compute line segment through (pr,pc) and (r,c)

		// special case of passages
	
		if line is horizontal or vertical:
			check each gridpoint for visibility
		else:
			for each row between pr and r:
				for each col between pc and j:
					check the grid points along the line for visibility
```

#### `grid_union`

```
	given two grids (grid1 and grid2), make grid1 the union of grid1 and grid2
	for each non-null char at (x,y) in grid2:
		overwrite char in grid1 with char of grid2
```

#### `grid_compressLayers`

```
	compress the given three grids into one grid
	zero out the compressedGrid
	transfer data of mapGrid into compressedGrid
	for each gold object in goldArray:
		if gold (x,y) is valid room spot AND directly visible:
			write an asterisk (*) to (x,y) on compressedGrid
	for each player object in playerArray:
		if player (x,y) is a valid room spot AND directly visible:
			write an asterisk (*) to (x,y) on compressedGrid
	return compressedGrid
```

---

## Testing plan

Refer to [Design Specification](DESIGN.md)

---

## Visibility Recap

To recap: the server has:

- a map-grid, of chars, which is static and loaded in from a map file

- a gold-grid, of `gold` objects

- a player-grid, of `player` objects

Each player object has (among other things), a `grid` that gets sent to the client for display

Our vision calculations occur between the main grids and the computed player grid

### Calculations Overview

After any player moves, we update our main grids, recalculate everyone’s grids, and broadcast the new grids to their respective players.

### Updating Main Grids

We update our main grids simply: if a player moves, it moves within the player-grid. If a player discovers gold, the gold object disappears from the gold grid.

### Spectator Grid

The spectator grid can see all gold and all players. So, the spectator’s grid is the main grid, overlaid by the gold grid, overlaid by the player grid. No information is hidden from the spectator.

### Player Grid

The player grid can only see the areas it has discovered, and any gold/players in direct visibility. Visibility calculations must be performed, adding newly found/seen regions to the player’s map grid. Gold and player grids must also undergo a vision algorithm. The player grid is then the calculated map grid, overlaid by the calculated gold grid, overlaid by the calculated player grid.

### The Vision Algorithm

When calculating the new map-grid for the player, we can calculate everything the player sees from its new spot. Then we combine the new vision (mathematical union) with the player’s previous map grid, creating a map of space the player has seen and just saw.

When calculating vision between a player and a gold or player grid, we calculate what the player has direct vision of.

---

## Limitations

> to be worked on as development progresses
