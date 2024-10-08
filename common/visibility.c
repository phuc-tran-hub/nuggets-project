/*
 * visibility.c - see visibility.h for more information on this module
 *
 * Ethan Thomas, 2021
 * Stanley Gao '24, CS50, 21F
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "grid.h"
#include "player.h"
#include "visibility.h"

#define VERT_BOUND '|'
#define HORIZ_BOUND '-'
#define CORNER_BOUND '+'
#define EMPTY_SPACE ' '
#define PASSAGE_SPOT '#'


/************** local functions **************/
static bool blocksVisionHorizontal(int r, int pc, int c, grid_t* mapGrid);
static bool blocksVisionVertical(int c, int pr, int r, grid_t* mapGrid);
static bool blocksVisionSloped(int pr, int pc, int r, int c, grid_t* mapGrid);
static bool checkRowIntersection(const int rise, const int run, int row, 
                                 int pr, int pc, grid_t* mapGrid);
static bool checkColumnIntersection(const int rise, const int run, int col, 
                                 int pr, int pc, grid_t* mapGrid);
static bool blockingCharacter(char c);

/************** visibility_calculateDirectVisibility **************/
/* see visibility.h for more details */
// PLAYER MUST BE x1, y1
bool
visibility_calculateDirectVisibility(grid_t* mapGrid, int x1, int y1, int x2, int y2)
{
  // slope calculations
  float rise = y1 - y2;
  float run = x1 - x2;

  // vertical case: check each y from y1 to y2
  if (run == 0) {
    return !blocksVisionVertical(x2, y1, y2, mapGrid);
  }

  // horizontal case: check each x from x1 to x2
  if (rise == 0) {
    return !blocksVisionHorizontal(y2, x1, x2, mapGrid);
  }
  
  // diagonal case: check rows, then check columns
  return !blocksVisionSloped(y2, x2, y1, x1, mapGrid);
  
  // backup: should never get here, but just say vision blocked if error
  printf("BIG BOY ERROR OOPSIES\n");
  return false;
}

/************** visibility_calculatePlayerVisibility **************/
/* see visibility.h for more details */
grid_t*
visibility_calculatePlayerVisibility(grid_t* mapGrid, player_t* player)
{
  // get the players current coordinates
  int pr = player_getY(player);
  int pc = player_getX(player);
  
  // get the size of the grid
  int rows = grid_getYLength(mapGrid);
  int cols = grid_getXLength(mapGrid);
  
  // make the resulting grid of visibility
  grid_t* visibleGrid = grid_makeEmptyGrid(cols, rows);

  // scan the grid
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      // if we are on the player spot, continue to next point
      if (pr == r && pc == c) {
        continue;
      }

      // if we are dealing with a horizontal line
      if (pr == r) {
        // call helper function for horizontal line
        bool blocksH = blocksVisionHorizontal(r, pc, c, mapGrid);

        // if vision is not blocked, add (c,r) to visible grid
        if (!blocksH) {
          // put (c,r) into visible grid
          char setChar = grid_getSpot(mapGrid, c, r);

          grid_setSpot(visibleGrid, c, r, setChar);
        }
      } 

      // else if we are dealing with a vertical line
      else if (pc == c) {
        // call helper function for vertical line
        bool blocksV = blocksVisionVertical(c, pr, r, mapGrid);

        // if vision is not blocked, add (c,r) to visible grid
        if (!blocksV) {
          // put (c,r) into visible grid
          char setChar = grid_getSpot(mapGrid, c, r);

          grid_setSpot(visibleGrid, c, r, setChar);
        }
      } 

      // else, we are dealing with a sloped line
      else {
        // call helper function for sloped line
        bool blocksS = blocksVisionSloped(pr, pc, r, c, mapGrid);
                
        // if vision is not blocked, add (c,r) to visible grid
        if (!blocksS) {
          // add (c,r) into visible grid
          char setChar = grid_getSpot(mapGrid, c, r);

          grid_setSpot(visibleGrid, c, r, setChar);
        }
      }
    }
  }

  return visibleGrid;
}


/************** blocksVisionHorizontal **************/
/*
 * helper function to determine if a point (c,r) lying on a horizontal line
 *    is visible to the player
 *
 * here, the row is constant, so we only need to check the points
 *    on the columns between pc and c
 *
 * returns: true if vision blocked at any point;
 *          false if vision not blocked
 */
bool
blocksVisionHorizontal(int r, int pc, int c, grid_t* mapGrid)
{
  // traverse leftward
  if (pc > c) {
    for (int x = pc - 1; x > c; x--) {
      // if the spot is not a room spot, vision is blocked
      char spotChar = grid_getSpot(mapGrid, x, r);

      if (blockingCharacter(spotChar)) {
        return true; // vision blocked => return true
      }
    }
  }

  // traverse rightward
  else {
    for (int x = pc + 1; x < c; x++) {
      // if the spot is not a room spot, vision is blocked
      char spotChar = grid_getSpot(mapGrid, x, r);

      if (blockingCharacter(spotChar)) {
        return true; // vision blocked => return true
      }
    }
  }

  return false; // vision is not blocked => return false
}


/************** blocksVisionVertical **************/
/*
 * helper function to determine if a point (c,r) lying on a vertical line
 *    is visible to the player
 *
 * here, the column is constant, so we only need to check the points
 *    on the rows between pr and r
 *
 * returns: true if vision blocked at any point;
 *          false if vision not blocked
 */
bool
blocksVisionVertical(int c, int pr, int r, grid_t* mapGrid)
{
  // traverse downward
  if (pr < r) {
    for (int y = pr + 1; y < r; y++) {
      // if the spot is not a room spot, vision is blocked
      char spotChar = grid_getSpot(mapGrid, c, y);

      if (blockingCharacter(spotChar)) {
        return true; // vision blocked => return true
      }
    }
  }

  // traverse upward
  else {
    for (int y = pr - 1; y > r; y--) {
      // if the spot is not a room spot, vision is blocked
      char spotChar = grid_getSpot(mapGrid, c, y);

      if (blockingCharacter(spotChar)) {
        return true; // vision blocked => return true
      }
    }
  }

  return false; // vision is not blocked => return false
}


/************** blocksVisionSloped **************/
/*
 * helper function to determine if a point (c,r) lying on a sloped line
 *    is visible to the player
 *
 * a Cartesian Grid reflected about the x-axis is used to determine how
 *    to iterate over the line (left to right or right to left). Our 
 *    plane is reflected to match the 2d array, where increasing y is
 *    downward and increasing x is to the right.
 *    
 *    III      IV
 *         |
 *         |
 *    -----+-----
 *         |
 *         |
 *    II        I
 *
 *    EX: if the player is to the left of and below the point of interest,
 *        we would be in the Quadrant IV
 *
 * returns: true if while checking along the line, vision is blocked;
 *          false if vision is not blocked
 */
static bool
blocksVisionSloped(int pr, int pc, int r, int c, grid_t* mapGrid)
{
  // represents the slope of the line segment
  const int rise = r - pr;
  const int run  = c - pc;
  
  if (rise < 0) { // Quadrants III & IV
    // traverse upward
    for (int y = pr - 1; y > r; y--) {
      // check where line intersects row
      bool checkRow = checkRowIntersection(rise, run, y, pr, pc, mapGrid);

      if (checkRow) {
        return true; // vision blocked => return true
      }
    }
    
    // traverse leftward
    if (run < 0) { // Quadrant III
      for (int x = pc - 1; x > c; x--) {
        // check where line intersects column
        bool checkCol = checkColumnIntersection(rise, run, x, pr, pc, mapGrid);

        if (checkCol) {
          return true; // vision blocked => return true
        }
      }
    }

    // traverse rightward
    else { // Quadrant IV
      for (int x = pc + 1; x < c; x++) {
        // check where line intersects column
        bool checkCol = checkColumnIntersection(rise, run, x, pr, pc, mapGrid);

        if (checkCol) {
          return true; // vision blocked => return true
        }
      }
    }
  }
  else { // Quadrants I & II
    // traverse downward
    for (int y = pr + 1; y < r; y++) {
      // check where line intersects row
      bool checkRow = checkRowIntersection(rise, run, y, pr, pc, mapGrid);

      if (checkRow) {
        return true; // vision blocked => return true
      }
    }

    // traverse leftward
    if (run < 0) { // Quadrant II
      for (int x = pc - 1; x > c; x--) {
        // check where line intersects column
        bool checkCol = checkColumnIntersection(rise, run, x, pr, pc, mapGrid);

        if (checkCol) {
          return true; // vision blocked => return true
        }
      }
    }

    // traverse rightward
    else {
      for (int x = pc + 1; x < c; x++) {
        // check where line intersects column
        bool checkCol = checkColumnIntersection(rise, run, x, pr, pc, mapGrid);

        if (checkCol) {
          return true; // vision blocked => return true
        }
      }
    }
  }

  return false; // vision not blocked => return false
}


/************** checkRowIntersection **************/
/*
 * finds the intersection of the line with a row and checks if vision is
 *    blocked at that row. If the intersection is not a gridpoint, the left
 *    and right neighbors must be checked: if they are both characters that
 *    block vision, vision is blocked.
 *
 * returns: true if vision is blocked;
 *          false if vision not blocked
 */
bool
checkRowIntersection(const int rise, const int run, int row, 
                     int pr, int pc, grid_t* mapGrid)
{
  // player's position as floats
  float x1 = pc;
  float y1 = pr;

  // coordinate of intersection point
  float x2;
  float y2 = row;

  float slope = (float)(rise) / (float)(run);

  x2 = (y2 - y1) / slope + x1;

  int checkInt = x2;
  float temp = x2 - checkInt;

  // now check if x2 is an integer
  if (temp == 0) {
    // simply check the point
    int x = x2;
    int y = y2;

    char c = grid_getSpot(mapGrid, x, y);

    if (blockingCharacter(c)) {
      return true; // vision blocked => return true
    }
  }
  else {
    // truncate x2 to get left neighbor
    // add 1 to truncated x2 to get right neighbor
    int left = x2;
    int right = left + 1;

    char leftChar  = grid_getSpot(mapGrid, left, row);
    char rightChar = grid_getSpot(mapGrid, right, row);

    // now check the neighbors
    if (blockingCharacter(leftChar) && blockingCharacter(rightChar)) {
      return true; // vision blocked => return true
    }
  }

  return false; // vision not blocked => return false
}


/************** checkColumnIntersection **************/
/*
 * finds the intersection of the line with a column and checks if vision is
 *    blocked at that column. If the intersection is not a gridpoint, the upper
 *    and lower neighbors must be checked: if they are both characters that
 *    block vision, vision is blocked.
 * 
 * returns: true if vision is blocked;
 *          false if vision not blocked
 */
bool
checkColumnIntersection(const int rise, const int run, int col, 
                        int pr, int pc, grid_t* mapGrid)
{
  // coordinate of player as floats
  float x1 = pc;
  float y1 = pr;

  // desired intersection coordinate
  float x2 = col;
  float y2;

  float slope = (float)(rise) / (float)(run);

  y2 = (slope) * (x2 - x1) + y1;

  int checkInt = y2;
  float temp = y2 - checkInt;

  // check if y2 is an integer
  if (temp == 0) {
    // simply check the spot
    int x = x2;
    int y = y2;

    char c = grid_getSpot(mapGrid, x, y);

    if (blockingCharacter(c)) {
      return true; // vision blocked => return true
    }
  } else {
    // get the neighbor coordinates
    int upper = y2;
    int lower = y2 + 1;

    char upperChar = grid_getSpot(mapGrid, col, lower);
    char lowerChar = grid_getSpot(mapGrid, col, upper);
    
    // check if vision is blocked
    if (blockingCharacter(upperChar) && blockingCharacter(lowerChar)) {
      return true; // vision blocked => return true
    }
  }

  return false; // vision not blocked => return false
}


/************** blockingCharacter **************/
/*
 * helper function to determine if a character in the grid blocks vision
 *
 * returns: false if it doesn't block;
 *          true if it does block;
 */
bool
blockingCharacter(char c)
{
  return ((c - VERT_BOUND)    == 0   || (c - HORIZ_BOUND) == 0 || 
          (c - CORNER_BOUND)  == 0   || (c - EMPTY_SPACE) == 0 || 
          (c - PASSAGE_SPOT) == 0);
}
