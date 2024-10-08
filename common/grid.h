/*
 * grid.h: implementation of structs and methods for grids
 *
 * Stanley Gao '24, CS50 21F, Nuggets Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ****************** grid struct ****************** */
/* 
 * struct holding information for a grid.
 * in our context, holding map/players/gold for the nuggets game.
 * Holds the grid's length and width sizes, and a 2D array of chars
 * 
 * the 2D array is visually structured as such (assuming a xLength=5, yLength=4 array):
 * (it is an array of arrays)
 * 
 * [y1] -> [x1,x2,x3,x4,x5]
 * [y2] -> [x1,x2,x3,x4,x5]
 * [y3] -> [x1,x2,x3,x4,x5]
 * [y4] -> [x1,x2,x3,x4,x5]
 * 
 * accessing the array without a getter requires array[y][x] format
 * accessing the array using a getter requires get(x, y) format
 */
typedef struct grid grid_t;

/* ****************** grid_makeEmptyGrid ****************** */
/* 
 * a function to create/allocate space for a new grid
 *
 * returns NULL if xLength or yLength are <1, or if memory errors 
 */
grid_t* grid_makeEmptyGrid(int xLength, int yLength);


/* ****************** grid_freeGrid ****************** */
/* 
 * a function to cleanly delete a given grid
 */
void grid_freeGrid(grid_t* grid);

/* ****************** grid_loadGridFromFile ****************** */
/* 
 * a function to create/allocate space for a new grid, loading in from a file
 *
 * returns NULL if file is unopenable/unreadable, if no text is in file (xLength of 0), memory errors
 */
grid_t* grid_loadGridFromFile(char* file);

/* ****************** grid_getXLength ****************** */
/* 
 * a simple getter to return the grid's xLength
 */
int grid_getXLength(grid_t* grid);

/* ****************** grid_getYLength ****************** */
/* 
 * a simple getter to return the grid's yLength
 */
int grid_getYLength(grid_t* grid);

/* ****************** grid_getData ****************** */
/* 
 * a simple getter to return the grid's 2d array (data)
 */
char** grid_getData(grid_t* grid);

/* ****************** grid_getSpot ****************** */
/* 
 * a simple getter to return the char at spot x,y
 *
 * returns EMPTY_GRID_CHAR if x,y out of bounds
 */
char grid_getSpot(grid_t* grid, int x, int y);

/* ****************** grid_checkSpot ****************** */
/* 
 * a helper/boolean function to determine if a spot is a given character on the grid
 *
 * returns NULL if x,y out of bounds
 */
bool grid_checkSpot(grid_t* grid, int x, int y, char checkChar);

/* ****************** grid_setSpot ****************** */
/* 
 * a function to set the char at spot x,y
 *
 * returns silently and early if setChar is '\0' ("null" character)
 * returns silently and early if x,y out of bounds
 */
void grid_setSpot(grid_t* grid, int x, int y, char setChar);

/* ****************** grid_union ****************** */
/* 
 * a function to union two grids: grid1 becomes union of grid1 and grid2
 * mainly used when combining a players current vision with its vision history
 * 
 * returns silently and early if grids are not the same size (no union is done)
 */
void grid_union(grid_t* grid1, grid_t* grid2);

/* ****************** grid_print ****************** */
/* 
 * a function to print a grid to output
 * for debugging mainly
 * can pipe the grid output to any kind of FILE* (stdout, etc.)
 */
void grid_print(grid_t* grid, FILE* output);

/* ****************** grid_toString ****************** */
/* 
 * a function to print a grid to a string
 * CALLER MUST FREE() THE STRING AFTER
 * 
 * returns NULL if memory errors
 */
char* grid_toString(grid_t* grid);