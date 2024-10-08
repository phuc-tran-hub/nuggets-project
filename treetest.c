// /* 
//  * treetest.c - test program for simple binary-tree module
//  *   (version 9: same as version 8)
//  *
//  * usage:
//  *   treetest weatherfile.xml...
//  * where the XML files are the result of downloads from
//  *  https://w1.weather.gov/xml/current_obs/XXXX.xml
//  * where XXXX is a weather station; e.g, Mount Washington is KMWN.
//  *
//  * David Kotz - April 2016, 2017, 2021
//  */

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "tree.h"
// #include "readlinep.h"

// struct weather {
//   char *location;             // descriptive name of the location
//   float tempF;                // temp in degrees F
//   float windspeed;            // wind speed
// };

// static void weatherprint(FILE *fp, const char *key, void *item);
// static void weatherdelete(void *item);

// int main(const int argc, const char *argv[]) 
// {
//   if (argc <= 1) {
//     fprintf(stderr, "usage: %s: weatherfile.xml...\n", argv[0]);
//     exit(1);
//   }

//   tree_t* tree;         // the tree

//   tree = tree_new();
//   if (tree == NULL) {
//     printf("tree_new failed\n");
//     exit(1);
//   }

//   printf("Looping over files:\n");
//   // read weather from each and insert each into the tree
//   for (int f = 1; f < argc; f++) {
//     char station[5] = "????";       // weather station (4 chars)

//     printf("\nFile %s\n", argv[f]);
//     FILE *fp = fopen(argv[f], "r"); // file with that station's data
//     if (fp == NULL) {
//       fprintf(stderr, "cannot open file '%s' for reading.\n", argv[f]);
//       continue;
//     }

//     // allocate a weather object to hold this file's data
//     struct weather *wp = malloc(sizeof(struct weather)); 
//     if (wp == NULL) {
//       fprintf(stderr, "malloc failed\n");
//       exit(3);
//     } else {
//       // initialize a weather struct
//       wp->location = malloc(1000); // big enough, I hope, for any location
//       wp->tempF = 0;
//       wp->windspeed = 0;
//     }

//     // read through the file, trying to find the data we need.
//     char* line;
//     while ( (line = freadLinep(fp)) != NULL) {
//       // try extracting the interesting fields from every line.
//       sscanf(line, "\t<station_id>%4s</station_id>", station);
//       sscanf(line, "\t<location>%[^<]</location>", wp->location);
//       sscanf(line, "\t<temp_f>%f</temp_f>", &(wp->tempF));
//       sscanf(line, "\t<wind_mph>%f</wind_mph>", &(wp->windspeed));
//       free(line);
//     }
//     fclose(fp);

//     // finished reading the file: print it and insert into tree.
//     printf("%s: %.1f F, %.1f mph (%s)\n", station, 
//            wp->tempF, wp->windspeed, wp->location);
//     if (!tree_insert(tree, station, wp)) {
//       fprintf(stderr, "error inserting %s into tree\n", station);
//     } else {
//       printf("inserted %s into tree\n", station);
//     }
//   }

//   printf("\ntesting tree_find:\n");
//   weatherprint(stdout, "KMWN", tree_find(tree, "KMWN")); putchar('\n');

//   printf("update node %s...", "KMWN");
//   if (!tree_insert(tree, "KMWN", NULL)) {
//     fprintf(stderr, "FAILED - insert should have allowed NULL data\n");
//   } else {
//     printf("SUCCESS\n");
//   }

//   printf("\ntesting tree_insert with NULLs:\n");
//   if (!tree_insert(tree, "KMWN", NULL)) {
//     fprintf(stderr, "error: tree_insert(tree, station, NULL) should be allowed\n");
//   }
//   if (tree_insert(tree, NULL, "foo")) {
//     fprintf(stderr, "error: tree_insert(tree, NULL, stp) should fail\n");
//   }
//   if (tree_insert(NULL, "KMWN", "foo")) {
//     fprintf(stderr, "error: tree_insert(NULL, symbol, stp) should fail\n");
//   }
//   printf("done.\n");

//   printf("\ntesting tree_find with NULLs:\n");
//   if (tree_find(tree, NULL)) {
//     fprintf(stderr, "error: tree_find(tree, NULL) should fail\n");
//   }
//   if (tree_find(NULL, "KMWN")) {
//     fprintf(stderr, "error: tree_find(NULL, \"KMWN\") should fail\n");
//   }
//   printf("done.\n");

//   printf("\nThe tree:\n");
//   tree_print(tree, stdout, weatherprint);

//   printf("\ndelete the tree...\n");
//   tree_delete(tree, weatherdelete);
//   printf("all done\n");

//   return 0;
// }

// /* *************************************************** */
// /* print the given item to the given file.
//  * in our test, key is the same as stp->symbol
//  */
// static void weatherprint(FILE *fp, const char *key, void *item)
// {
//   if (fp != NULL) {
//     struct weather *wp = item;
//     if (wp == NULL) {
//       fprintf(fp, "[(null)]");
//     } else {
//       fprintf(fp, "[%s: %.1f F, %.1f mph (%s)]", key, 
//               wp->tempF, wp->windspeed, wp->location);
//     }
//   }
// }

// /* *************************************************** */
// /* delete a weather item */
// static void weatherdelete(void *item)
// {
//   struct weather *wp = item;

//   if (wp != NULL) {
//     if (wp->location != NULL) {
//       free(wp->location);
//     }
//     free(wp);
//   }
// }
