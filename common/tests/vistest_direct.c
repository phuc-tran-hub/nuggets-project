/*
 * gridtest.c
 * test driver for development of grid.c
 * 
 * Stanley Gao '24, CS50 21F, Nuggets Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../visibility.h"

int main(int argc, char const *argv[])
{	
	grid_t* grid = grid_loadGridFromFile("main.txt");

	// grid_setSpot(grid, 4, 2, 'A');
	// grid_setSpot(grid, 8, 2, '*');

	/*
	bool result = visibility_calculateDirectVisibility(grid, 4, 2, 8, 2);
	printf("horiz: %d\n", result);

	result = visibility_calculateDirectVisibility(grid, 4, 1, 8, 2);
	printf("sloped: %d\n", result);

	result = visibility_calculateDirectVisibility(grid, 4, 1, 4, 3);
	printf("vert: %d\n", result);
	*/
	
	// x2,y2 (8,5) is out of bounds, should be not visible
	// x2,y2 (8,2) is horizontal, should be visible
	// x2,y2 (8,3) is sloped, should be visible 
	// x2,y2 (7,2) is vertical, should be visible

	int x1 = 5; int y1 = 2;
	int x2 = 7; int y2 = 2;

	grid_setSpot(grid, x1, y1, 'A');
	grid_setSpot(grid, x2, y2, '*');
	grid_print(grid, stdout);

	printf("Result: %d\n", visibility_calculateDirectVisibility(grid, x1, y1, x2, y2));
	printf("[0 = NOT directly visible, 1 = directly visible]\n");

	grid_freeGrid(grid);
	return 0;
}
