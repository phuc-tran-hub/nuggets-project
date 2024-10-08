/*
 * grid.c
 * see grid.h for detailed documentation
 * 
 * Stanley Gao '24, CS50 21F, Nuggets Project
 */

#include "file.h"
#include "grid.h"

static char EMPTY_GRID_CHAR = ' ';
static char GOLD_CHAR = '*';

/* ****************** grid struct ****************** */
// see grid.h for detailed documentation
typedef struct grid {
	int xLength;
	int yLength;
	char** data;  // represents the map of entities
} grid_t;

/* ****************** getter functions ****************** */
/* getXLength */
int
grid_getXLength(grid_t* grid)
{
  return grid ? grid->xLength : -1;
}
/* getYLength */
int
grid_getYLength(grid_t* grid)
{
  return grid ? grid->yLength : -1;
}
/* getData */
char**
grid_getData(grid_t* grid)
{
  return grid->data;
}

/* ****************** grid_makeEmptyGrid ****************** */
// see grid.h for detailed documentation
grid_t*
grid_makeEmptyGrid(int xLength, int yLength)
{
  if (xLength < 1 || yLength < 1) {
    return NULL;
  }

  grid_t* grid = malloc(sizeof(grid_t));
  if (grid == NULL) {
    return NULL;
  }
  grid->xLength = xLength; 
  grid->yLength = yLength;

  // make 1 column of size yLength, with pointers to char*
  char** arr = calloc(yLength, sizeof(char*));
  if (arr == NULL) {
    return NULL;
  }

  // make yLength rows of size xLength, with char 
  for (int i = 0; i < yLength; i++)
  {
      arr[i] = calloc(xLength + 1, sizeof(char)); // + 1 for terminating \0 char
      if (arr[i] == NULL) {
        return NULL;
      }

      memset(arr[i], EMPTY_GRID_CHAR, xLength);
  }
  grid->data = arr; // data[y][x] works

  // return produced grid
  return grid;
}

/* ****************** grid_freeGrid ****************** */
// see grid.h for detailed documentation
void
grid_freeGrid(grid_t* grid)
{
  // free each row
  for (int i = 0; i < grid->yLength; i++)
  {
      free(grid->data[i]);
  }
  // free the column of size yLength
  free(grid->data);
  // free the grid object
  free(grid);
}

/* ****************** grid_loadGridFromFile ****************** */
// see grid.h for detailed documentation
grid_t* grid_loadGridFromFile(char* file)
{
  // get dimensions of our new grid
  FILE* fp = fopen(file, "r");
  if (fp == NULL) {
    return NULL;
  }
  int numLines = file_numLines(fp); // grid y
  char* line = file_readLine(fp);
  if (line == NULL) {
    return NULL;
  }
  int lineLength = strlen(line); // grid x
  free(line);
  fclose(fp);

  grid_t* grid = grid_makeEmptyGrid(lineLength, numLines);
  if (grid == NULL) {
    return NULL;
  }

  fp = fopen(file, "r");
  int y = 0;
  while (line = file_readLine(fp), line != NULL) {
    strcpy(grid->data[y], line);
    y++;
    free(line);
  }

  fclose(fp);
  return grid;
}

/* ****************** grid_getSpot ****************** */
// see grid.h for detailed documentation
char
grid_getSpot(grid_t* grid, int x, int y)
{
  if (x < 0 || y < 0 || x >= grid->xLength || y >= grid->yLength) {
    return EMPTY_GRID_CHAR;
  }
  return grid->data[y][x];
}

/* ****************** grid_checkSpot ****************** */
// see grid.h for detailed documentation
bool grid_checkSpot(grid_t* grid, int x, int y, char checkChar)
{
  if (x < 0 || y < 0 || x >= grid->xLength || y >= grid->yLength) {
    return NULL;
  }
  if (grid_getSpot(grid, x, y) == checkChar) {
    return true;
  }
  return false;
}

/* ****************** grid_setSpot ****************** */
// see grid.h for detailed documentation
void
grid_setSpot(grid_t* grid, int x, int y, char setChar)
{
  if (setChar == '\0') {
    return;
  }
  if (x < 0 || y < 0 || x >= grid->xLength || y >= grid->yLength) {
    return;
  }
  grid->data[y][x] = setChar; 
}

/* ****************** grid_union ****************** */
// see grid.h for detailed documentation
void
grid_union(grid_t* grid1, grid_t* grid2)
{
  // assert grids are of same size
  if (grid1->xLength != grid2->xLength || grid1->yLength != grid2->yLength) {
    return;
  }

  for (int y = 0; y < (grid2->yLength); y++) {
    for (int x = 0; x < grid2->xLength; x++) {
      if ((grid2->data[y][x]) != EMPTY_GRID_CHAR) {
        grid1->data[y][x] = grid2->data[y][x];
      }
      if ((grid2->data[y][x]) == GOLD_CHAR || (grid1->data[y][x]) == GOLD_CHAR) {
        grid1->data[y][x] = GOLD_CHAR;
      }
    }
  }
}

/* ****************** grid_print ****************** */
// see grid.h for detailed documentation
void
grid_print(grid_t* grid, FILE* output)
{
  char* outputString = grid_toString(grid);
  fprintf(output, "%s", outputString);
  free(outputString);
}

/* ****************** grid_toString ****************** */
// see grid.h for detailed documentation
char*
grid_toString(grid_t* grid)
{
  int mallocMath = ((grid->xLength + 1) * grid->yLength) + 1;
  char* outputString = malloc(mallocMath * sizeof(char));
  if (outputString == NULL) {
    return NULL;
  }
  strcpy(outputString, "");
  
  for (int y = 0; y < grid->yLength; y++) {
    strcat(outputString, grid->data[y]);
    strcat(outputString, "\n");
  }
  
  return outputString;
}
