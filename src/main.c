/*
Desmos in your terminal, you absolutely don't need it
*/

#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include "cartesian-plane.h"
#include <locale.h>

void draw_menu(); 
WINDOW* init_menu_win();

int main() {

    //Enable UTF-8 Locale (for title art)
    setlocale(LC_ALL, ""); 
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
    init_pair(2, COLOR_CYAN, COLOR_BLACK); 

    /* Initalize cartesian plane */
    Plane *p = init_plane(); 

    int max_x, max_y, prev_max_x, prev_max_y;
    int ch; 

    draw_menu(); 
    refresh(); 
    wrefresh(p->win); 

    while (true)
    {
        getmaxyx(stdscr, max_y, max_x);
        mvprintw(0, max_x-XPADDING -12, "SCALE: %.2f", p->scale); 

        /* Draw plane and graph*/
        clear_plane(p); 
        draw_plane(p);
        // draw_parabola(p); 
        wrefresh(p->win); 

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
        if (prev_max_x != max_x || prev_max_y != max_y)
        {
            clear(); 
            draw_menu(); 
            updateCenter(p); 
            refresh(); 
        }
    }

    endwin();
    return 0;
}

//Draw menu on main window
void draw_menu()
{
    int max_x, title_row; 
    max_x = getmaxx(stdscr); 
    /* Title card setup*/
    char* title_line1 = "░█▀█░█▀▀░█▀▀░▀█▀░▀█▀░█▀█░█░░░█▀█░▀█▀";
    char* title_line2 = "░█▀█░▀▀█░█░░░░█░░░█░░█▀▀░█░░░█░█░░█░"; 
    char* title_line3 = "░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░░░▀▀▀░▀▀▀░░▀░"; 
    
    attron(COLOR_PAIR(2)); 
    attron(A_BOLD); 
    
    mvprintw(YPADDING/2-1, XPADDING, "Function: ");
    
    attron(A_BOLD); 
    attroff(COLOR_PAIR(2)); 

    /* TITLE CARD*/
    title_row = max_x/2 - 19; 
    mvprintw(1,title_row, "%s", title_line1); 
    mvprintw(2, title_row, "%s", title_line2); 
    mvprintw(3,title_row, "%s", title_line3); 

    /*Top right corner TIPS*/
    const char* helpmsg = "Arrow keys or touch pad to move around graph";
    const char* zoommsg = "pgup and pgdown keys to zoom into graph"; 
    attron(A_BOLD); 
    mvprintw(5, max_x-XPADDING -strlen(helpmsg)/2, "TIPS"); 
    attroff(A_BOLD); 
    mvprintw(6, max_x-XPADDING - strlen(helpmsg), "%s",helpmsg); 
    mvprintw(7, max_x-XPADDING - strlen(zoommsg), "%s", zoommsg); 

}