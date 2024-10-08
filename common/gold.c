/*
 * gold.c -> see common.h for detailed documentation
 * 
 * Henry Bonnie   Nov. 7th   CS50 F21
 * Stanley Gao '24, CS50 21F
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gold.h"
#include "grid.h"

/* ****************** gold ****************** */
/* see gold.h for detailed documentation */
typedef struct gold {
  int x; // x location of gold
  int y; // y location of gold
  int goldStored; // gold stored within object
} gold_t;

/* ****************** getter functions ****************** */
/* getX */
int
gold_getX(gold_t* gold)
{  return gold ? gold->x : -1; }

/* getY */
int
gold_getY(gold_t* gold)
{  return gold ? gold->y : -1; }

/* ****************** gold_new ****************** */
/* see gold.h for detailed documentation */
gold_t*
gold_new(int gold, int x, int y)
{
  gold_t* newGold = (gold_t*)malloc(sizeof(int)*3); // allocate memory for gold object
  // check pointer for null
  if (newGold == NULL) {
    return NULL;
  }
  newGold->x = x; // set x location
  newGold->y = y; // set y location
  newGold->goldStored = gold; // set gold value
  return newGold;
}

/* ****************** gold_getGold ****************** */
/* see gold.h for detailed documentation */
int
gold_getGold(gold_t* gold)
{
  // check pointer for null
  if (gold == NULL) {
    return -1;
  }
  return gold->goldStored; // return gold stored
}

void
gold_setGold(gold_t* gold, int new)
{
 if (gold == NULL) {
   exit(1);
  }
 gold->goldStored = new;
}


/* ****************** gold_delete ****************** */
/* see gold.h for detailed documentation */
void
gold_delete(gold_t* gold)
{
  // check pointer for null
  if (gold == NULL) {
    return;
  }
  free(gold); // free gold object from memory
}



