# CS50 Nuggets
## Common

This directory serves as the common library used by the server to run the Nuggets game. Common contains several modules archived into `common.a`:

 * `grid`, which contains the 2D array and related details
 * `gold`, which contains the object used in the game for gold
 * `player`, which contains the object used in the game for players
 * `visibility`, which handles visibility calculations using grids, gold objects, and player objects
 * `compress`, which converts grids, gold arrays, and player arrays into one grid/one string for communication
 * `game`, which holds details related to the game – including grids, players, gold objects, etc.
 * `file`, which comes from libcs50

To compile, simply run `make` to produce `common.a`.

Additionally, this directory contains a folder, `tests`, which contains files that attempt to validate/ modules.
