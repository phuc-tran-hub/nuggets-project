/*
 * curses example - draw on a screen
 * 
 * compile with
 *   mygcc curse.c -lcurses -o curse
 *
 * David Kotz, May 2017
 */

#include <ncurses.h>
#include <ctype.h>

#define CTRL(c)  ((c)-0100)  // control character
static inline int min(int a, int b) { return a < b ? a : b; }
static inline int max(int a, int b) { return a > b ? a : b; }

int
main(int argc, char* argv[])
{
  // initialize ncurses
  initscr(); // initialize the screen
  cbreak();  // accept keystrokes immediately
  noecho();  // don't echo characters to the screen
  start_color(); // turn on color controls
  init_pair(1, COLOR_RED, COLOR_BLACK); // define color 1
  attron(COLOR_PAIR(1)); // set the screen color using color 1
  printw("Back with ^B, forward with ^F\n");
  printw("Up with ^P, down with ^N\n");
  printw("Beginning of line with ^A\n");
  printw("Exit with ^X\n");
  attroff(COLOR_PAIR(1)); // turn off the screen color

  // get the boundaries of the screen
  int ly, lx; // upper left
  getbegyx(stdscr, ly, lx);
  int uy, ux; // lower right
  getmaxyx(stdscr, uy, ux);

  // move to center of screen
  int x = (ux-lx)/2+lx;
  int y = (uy-ly)/2+ly;
  move(y,x);

  // steer cursor with emacs-like control characters
  int c;
  while ((c = getch()) != CTRL('X')) {    // read one character
    switch(c) {
    case CTRL('B'):   move(y,--x); break; // move cursor left
    case CTRL('F'):   move(y,++x); break; // move cursor right
    case CTRL('P'):   move(--y,x); break; // move cursor up
    case CTRL('N'):   move(++y,x); break; // move cursor down
    case CTRL('A'):   move(y,x=0); break; // move cursor far left
    default: if (isprint(c)) { addch(c); ++x; }  // add character at cursor
    }

    // keep the cursor in bounds
    x = min(x,ux); x = max(x,lx);
    y = min(y,uy); y = max(y,ly);

    refresh(); // redraw the screen
  }

  endwin(); // turn off curses display
  printf("Goodbye\n"); // prints to stdout as usual.
}
