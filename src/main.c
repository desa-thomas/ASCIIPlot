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
    start_color();          //enable colors
    //nodelay(stdscr, true); //make getch non-blocking

    /* Initialize colors */
    init_pair(1, COLOR_RED, COLOR_BLACK);

    /* Initalize cartesian plane */
    Plane *p = init_plane(); 

    int ch, max_x, max_y, prev_max_x, prev_max_y;
    getmaxyx(stdscr, max_y, max_x);

    while (true)
    {
        /*TODO*/
        //parse function input (handle formatting, use library or create parser)
        //graph function
        
        mvprintw(YPADDING/4, XPADDING/2, "1: ");

        clear(); 
        mvprintw(0, max_x-XPADDING/2 -10, "SCALE: %f", p->scale); 
        /* Draw viewbox and plane*/
        draw_box(p); 
        draw_plane(p);
        draw_parabola(p); 
        refresh();

        /* If ctrl-c is pressed, quit*/
        ch = getch(); 
        if (ch == 3)
        {
            break;
        }
        /* If arrow keys are pressed, move view*/
        else 
        {
            /* Arrow keys for moving around graph*/
            if (ch == KEY_UP)
                move_origin(p, DOWN); 
            else if (ch == KEY_DOWN)
                move_origin(p, UP); 
            else if (ch == KEY_LEFT)
                move_origin(p, RIGHT); 
            else if (ch == KEY_RIGHT) 
                move_origin(p, LEFT); 

            /* Page up and Page down for zooming*/
            else if (ch == KEY_NPAGE)
                zoom(p, ZOOMOUT); 
                
            else if (ch == KEY_PPAGE)
                zoom(p, ZOOMIN); 
                

            mvprintw(0,0,"%c", ch); 
        }

        /* If screen size changes*/
        prev_max_x = max_x; 
        prev_max_y = max_y; 
        getmaxyx(stdscr, max_y, max_x);
        if (prev_max_x != max_x || prev_max_y != max_y) updateCenter(p); 
    }

    endwin();
    return 0;
}