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
    Plane *p = init_plane(100, 35, 5.0, 75, 20); 
    int ch;
    while (true)
    {
        mvprintw(0, 0, "press esc then enter function: "); 
        draw_box(p); 
        draw_plane(p);
        refresh();

        ch = getch(); 
        if (ch == 3)
        {
            break;
        }
        else 
        {
            if (ch == KEY_UP)
                move_origin(p, DOWN); 
            else if (ch == KEY_DOWN)
                move_origin(p, UP); 
            else if (ch == KEY_LEFT)
                move_origin(p, RIGHT); 
            else if (ch == KEY_RIGHT) 
                move_origin(p, LEFT); 
            clear(); 
        }
        
    }

    endwin();
    return 0;
}