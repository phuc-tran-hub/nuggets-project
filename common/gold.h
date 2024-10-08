/*
 * gold.h -> implementation of gold object and related functions
 *
 * Henry Bonnie   Nov. 7th   CS50 F21
 * Stanley Gao '24, CS50 21F
 */


/* ****************** gold ****************** */
/* 
 * gold -> struct to represent gold within the game. Holds an
 *  x and y location of the gold as well as the amount of gold
 *  stored on the specific spot
 */
typedef struct gold gold_t;

/* ****************** getter functions ****************** */
/* getX */
int gold_getX(gold_t* gold);

/* getY */
int gold_getY(gold_t* gold);

/* getGold */
int gold_getGold(gold_t* gold);

/* ****************** setter functions ****************** */
/* setGold */
void gold_setGold(gold_t* gold, int new);

/* ****************** gold_new ****************** */
/* 
 * gold_new -> initializes and returns a new gold struct.
 *  Takes three integers as parameters, an x location,
 *  a y location, and the amount of gold stored on the spot.
 * 
 *  Returns a pointer to the gold object if successfully
 *  initialized, otherwise returns a NULL gold object
 */
gold_t* gold_new(int gold, int x, int y);

/* ****************** gold_getGold ****************** */
/* 
 * gold_getGold -> getter function that returns the amount
 *  of gold being held within a gold object. Takes gold as 
 *  parameter and returns its gold stored value
 * 
 * returns -1 if gold object is NULL
 */
int gold_getGold(gold_t* gold);

/* ****************** gold_delete ****************** */
/* 
 * gold_delete -> function to free a gold object from memory,
 *  takes gold object you want freed as parameter and frees from 
 *  memory
 * 
 * returns silently and early if gold object is NULL (does not need to be freed)
 */
void gold_delete(gold_t* gold);

