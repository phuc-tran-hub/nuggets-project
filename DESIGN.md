# CS50 Nuggets
## Design Spec
### take-it-to-emacs, Fall, 2021

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes 1 module: `grid`.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

./client [hostname] [port] [optional: playername]

Where:
 - `hostname` is the name of the server host
 - `port` is the port number provided by the server
 - `playername` is the players name; if omitted, the client is a spectator

#### Gameplay

Valid keys:

* `Q` quit the game.

* `h` move left, if possible

* `l` move right, if possible

* `j` move down, if possible

* `k` move up  , if possible

* `y` move diagonally up and left, if possible

* `u` move diagonally up and right, if possible

* `b` move diagonally down and left, if possible

* `n` move diagonally down and right, if possible

* Any of the above move keys, if capitalized, attempt to move repeatedly and automatically in its respective direction until it can't

### Inputs and outputs

*Input:* Client takes command-line parameters described above. Client takes no other input from stdin; it however does receive messages sent by connected clients.

*Output:* Client outputs messages received from the server to stderr.

### Functional decomposition into modules

The client is a simple program which only needs to display the map sent by the server, and send keystrokes to server indicating moves.

*main*: parses arguments, initializes player struct, and sends keystrokes inputted by user to the server

*displayMap*: displays the map sent by the server
 
### Pseudo code for logic/algorithmic flow

```
parse arguments
connect to server
while player hasn’t quit game
	if player types key:
		send message to server
		parse received message from server
		if message == OK
			initialize player struct
		if message == GRID
			initialize size of grid to print
		if message == GOLD
			Update gold on screen
		if message == DISPLAY
			update display using received map character string
		if message == QUIT
			break while loop
		if message == ERROR
			print error to stderr
```

### Major data structures

`player` will be the main struct within client and our global variable. The client is dumb and only needs to hold a few pieces of info that will be held within player. It will hold a player ID, a purse of hold, and whether it is a spectator or not.

In order to interact with the server, the client will use functions provided by `message.c`.

## Server
### User interface

./server [map.txt] [seed]

Where:
 - map.txt is a text file containing the map for the game
 - seed: seed is an optional second argument containing the seed number for the random number generator.

### Inputs and outputs

*Input:* Server takes command-line parameters described above. Server takes no other input from stdin; it however does receive messages sent by connected clients.

*Output:* Server will print nothing to stdout. However, it sends messages to clients as part of game communication.

### Functional decomposition into modules

We anticipate the following modules or functions:

*main*: parses arguments, initializes server, and handles game logic

*handleGame*: handles messages from clients and sends messages to clients

*addVisibility*: determine what is visible to a player and update their unique map

And the following modules to provide data structures

*grid*: used to represent the game map and the entities within. Holds much of the information necessary to gameplay.

### Pseudo code for logic/algorithmic flow

```
parse arguments
load grid from map.txt
initialize game_t* game
populate grid with gold
begin server
while there is gold remaining
	receive message from clients
	processMessage
	send response message & map to client
print game over/winner
clean up
```

### Major data structures

#### game

Our main data structure is `game`, which hosts many variables and structs related to the game itself. The `game` struct will likely be our one global variable for our programs.

`game` includes a master `grid`, which represents the base map. This remains unchanged throughout gameplay, and is loaded in from map files.

`game` includes lists of `gold` and `player` objects. These are lists of pointers to gold/player objects.

`game` includes a `grid` of `gold` objects, which are populated according to the gold constants. As gold is found by players, `gold` objects get removed from the `grid`'s array. (A 2D interpretation of the `gold` object list).

`game` includes a `grid` of `player` objects, which are populated as clients join the game. As players move within the game, their player object changes location in the `grid`'s array. Each `player` object has its own `grid`, of what the client should display. (A 2D interpretation of the `player` object list).

`game` can also include other variables that would be helpful on a global scale (such as an integer for gold remaining across the entire map). We can discover these as the development process progresses.

#### player

The `player` struct includes a char of its player ID (A thru Z), integers for its location in the grid, an integer for its purse, and a `grid` object of its own visible grid. The server calculates this grid based on the vision algorithm.

The spectator has player ID "0", location (x,y) of (-1, -1), a purse quantity of -1, and its own grid object (like any other `player`).

#### gold

The `gold` struct includes integers for its location in the grid, and an integer for the quantity of gold in the pile. This integer is assigned by pseudo-random generation in a populateGold-esque function bounded by the global gold constants (GoldTotal, GoldMinNumPiles, GoldMaxNumPiles).

## Grid module

### grid

The `grid` struct comes from an abstracted module that represents a game grid.

 The `grid` struct holds grid length and width integers, as well as a 2D array of `void* items`.

These `void* items` can be characters (for the base map), or gold structs (for the gold grid), or player structs (for the player grid).

`grid` will include methods to load from map files and save/compress memory-grids to string outputs (usually, when messaging to clients).

### Functional decomposition

*grid_makeEmptyGrid* - creates empty grid

*grid_loadMap* - filling empty grid according to a character string map input

*grid_loadGoldMap* - fills in a grid of gold objects

*grid_loadPlayerMap* - fills in a grid of players

*grid_compressLayers* - compresses the base, gold, and player grids into one grid to be displayed

*grid_delete* - deletes the grid

### Pseudo code for logic/algorithmic flow

`grid_makeEmptyGrid`:
```
  initialize a 2d array
```

`grid_loadMap`:
```
  takes a map and fills in a grid line by line with the boundaries, spots, and passages of the map
```

`grid_loadGoldMap`:
```
  takes an array of gold structs and populates an empty grid with '*' where the gold is to be placed
```

`grid_loadPlayerMap`:
```
  takes an array of player structs and populates an empty grid with [char] at a location of a player
```

`grid_compressLayers`:
```
  for a unique players grid, compress the base map, gold map, and players map into one grid
```

### Major data structures

`gold struct`: represents a piece of gold

`player struct`: represents a player

`2d array`: represents the map

## Testing Plan

### unit testing
For unit testing we will test the functionality of all the major functions within the client, server, and grid module. We will test functionality of specific functions as well as ability to deal with erroneous cases.

### integration testing
The client/server module will be individually tested by using the provided, functioning version of the of its opposite module. This way we can locate bugs specific to the client or server module. Along with this we will test the client/server's ability to parse and correctly accommodate each message. We will use our client/server to test the user experience and functionality when run by a human. This will demonstrate how all the functions within the client/server work cohesively.

### system testing
Our system testing will be similar to our integration testing, but using our own server and client module to verify that they are cohesive with one another. This way we can locate bugs that arise when the two modules run together and potential asymmetries between them.

## Team Roles within Module
* grid: Stanley Gao, Ethan Thomas
    Testing: Henry Bonnie, Phuc Tran
* visibility:  Stanley Gao, Ethan Thomas
    Testing: Henry Bonnie, Phuc Tran
* server: Henry Bonnie, Phuc Tran
    Testing: Stanley Gao, Ethan Thomas
* Client: Henry Bonnie, Phuc Tran
    Testing: Ethan Thomas, Stanley Gao

