/*
Desmos in your terminal, you absolutely don't need it
*/

#include <ncurses.h>
#include <stdbool.h>
#include "cartesian-plane.h"

int main() {

    /* Initialize ncurses */

    initscr();              //init screen
    raw();                  //enable raw mode (how to handle ctrl c)
    noecho();               //disable echoing
    keypad(stdscr, true);   //enable keypad
    curs_set(0);            //make cursor invisible
    //nodelay(stdscr, true); //make getch non-blocking

    /* Initalize cartesian plane */
    Plane *p = init_plane(100, 30, 5.0, 75, 20); 

    int ch;
    while (true)
    {
        draw_plane(p);

        ch = getch(); 
        if (ch == 3)
        {
            break;
        }
        else 
        {
            //pass
        }
        
    }

    endwin();
    return 0;
}