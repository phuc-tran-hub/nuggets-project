/*
 * gridtest.c
 * test driver for development of grid.c
 * 
 * Stanley Gao '24, CS50 21F, Nuggets Project
 * Phuc Tran '24, modified for a comprehensive test
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../grid.h"
#include "file.h"

int main(int argc, char const *argv[])
{	

	fprintf(stdout, "--- START grid 1 --------------\n");
	// Testing grid1
	fprintf(stdout, "Create first grid...\n");
	grid_t* grid1 = grid_makeEmptyGrid(10, 10);

	// Testing make_emptyGrid
	if (grid1 == NULL){
		fprintf(stdout, "grid_makeEmptyGrid failed for grid1\n");
	} else{
	fprintf(stdout, "grid_makeEmptyGrid successful for grid1\n");
	}

	grid_freeGrid(grid1);
	fprintf(stdout, "--- END grid 1 --------------\n\n");


	fprintf(stdout, "--- START grid 2 --------------\n");
	// Testing grid2 
	fprintf(stdout, "Create second grid...\n");
	grid_t* grid2 = grid_loadGridFromFile("main.txt");

	// Testing make_emptyGrid
	if (grid2 == NULL){
		fprintf(stdout, "grid_loadGridFromFile failed for grid2\n");
	} else{
	fprintf(stdout, "grid_loadGridFromFile successful for grid2\n");
	}

	// Testing getters for grid2
	int xLengthGrid2 = grid_getXLength(grid2);
	int yLengthGrid2 = grid_getYLength(grid2);

	// Testing grid_xLengthGrid
	if (xLengthGrid2 == 0){
		fprintf(stdout, "grid_getXLength failed for grid2\n");
	} else{
		fprintf(stdout, "grid2's xLength: %d\n", xLengthGrid2);
		fprintf(stdout, "grid_getXLength successful for grid2\n");
	}

	// Testing grid_yLengthGrid
	if (yLengthGrid2 == 0){
		fprintf(stdout, "grid_getYLength failed for grid2\n\n");
	} else{
		fprintf(stdout, "grid2's yLength: %d\n", yLengthGrid2);
		fprintf(stdout, "grid_getYLength successful for grid2\n\n");
	}

	// These lengths are valid since this is what the example server provides for the same map 

	// Testing getSpot
	char spot1 = grid_getSpot(grid2, 0, 0);
	char spot2 = grid_getSpot(grid2, 0, 1);
	char spot3 = grid_getSpot(grid2, 1, 1);
	char spot4 = grid_getSpot(grid2, 1, 3);

	// Testing spot1
	if (spot1 == '0'){
		fprintf(stdout, "grid_getSpot failed for an empty spot\n");
	} else{
	fprintf(stdout, "grid_getSpot successful for an empty spot\n");
	}

	// Testing spot2
	if (spot2 == '0'){
		fprintf(stdout, "grid_getSpot failed for a corner spot\n");
	} else{
	fprintf(stdout, "grid_getSpot successful for a corner spot\n");
	}

	// Testing spot3
	if (spot3 == '0'){
		fprintf(stdout, "grid_getSpot failed for a boundary spot\n");
	} else{
	fprintf(stdout, "grid_getSpot successful for a boundary spot\n");
	}

	// Testing spot4
	if (spot4 == '0'){
		fprintf(stdout, "grid_getSpot failed for an available spot\n\n");
	} else{
	fprintf(stdout, "grid_getSpot successful for an available spot\n\n");
	}

	// Testing checkSpot
	char checkSpot1 = grid_checkSpot(grid2, 21, 15, ' ');
	char checkSpot2 = grid_checkSpot(grid2, 1, 0, '+');
	char checkSpot3 = grid_checkSpot(grid2, 1, 1, '|');
	char checkSpot4 = grid_checkSpot(grid2, 2, 1, '.');


	// Testing checkSpot1
	if ((int)checkSpot1 != atoi(" ")){
		fprintf(stdout, "grid_checkSpot failed for an empty spot\n");
	} else{
		fprintf(stdout, "grid_checkSpot successful for an empty spot\n");
	}

	// Testing checkSpot2
	if ((int)checkSpot2 != atoi("+")){
		fprintf(stdout, "grid_checkSpot failed for an corner spot\n");
	} else{
		fprintf(stdout, "grid_checkSpot successful for an corner spot\n");
	}

	// Testing checkSpot3
	if ((int)checkSpot3 != atoi("|")){
		fprintf(stdout, "grid_checkSpot failed for an boundary spot\n");
	} else{
		fprintf(stdout, "grid_checkSpot successful for an boundary spot\n");
	}

	// Testing checkSpot4
	if ((int)checkSpot4 != atoi(".")){
		fprintf(stdout, "grid_checkSpot failed for an available spot\n");
	} else{
		fprintf(stdout, "grid_checkSpot successful for an available spot\n");
	}

	grid_freeGrid(grid2);
	fprintf(stdout, "--- END grid 2 --------------\n");

	fprintf(stdout, "--- START grid 3 --------------\n");
	fprintf(stdout, "--- START grid 4 --------------\n");

	// Testing grid3
	fprintf(stdout, "Create third grid...\n");
	grid_t* grid3 = grid_loadGridFromFile("small1.txt");

	// Testing make_emptyGrid
	if (grid3 == NULL){
		fprintf(stdout, "grid_loadGridFromFile failed for grid3\n");
	} else{
		fprintf(stdout, "grid_loadGridFromFile successful for grid3\n");
		grid_print(grid3, stdout);
	}

	// Testing grid4
	fprintf(stdout, "Create fourth grid...\n");
	grid_t* grid4 = grid_loadGridFromFile("small2.txt");

	// Testing make_emptyGrid
	if (grid4 == NULL){
		fprintf(stdout, "grid_loadGridFromFile failed for grid4\n");
	} else{
		fprintf(stdout, "grid_loadGridFromFile successful for grid4\n");
		grid_print(grid4, stdout);
	}

	fprintf(stdout, "Combining third and fourth grid...\n");
	grid_union(grid3, grid4);
	grid_print(grid3, stdout);

	// Testing grid_toString
	fprintf(stdout, "Testing grid_toString...\n");
	char* outputString = grid_toString(grid3);
	if (outputString == NULL){
		fprintf(stdout, "grid_toString failed for grid3\n");	
	} else{ 
		fprintf(stdout, "grid_toString successful for grid3\n");	
		fprintf(stdout, "%s", outputString);
	}
	free(outputString);

	// Testing grid_setSpot
	fprintf(stdout, "Testing grid_setSpot...\n");
	fprintf(stdout, "Inserting 'C' at 3,1 (y, x)...\n");
	grid_setSpot(grid3, 3, 1, 'C');
	grid_print(grid3, stdout);

	grid_freeGrid(grid3);
	grid_freeGrid(grid4);
	fprintf(stdout, "--- END grid 3 --------------\n");
	fprintf(stdout, "--- END grid 4 --------------\n");

	return 0;
}
