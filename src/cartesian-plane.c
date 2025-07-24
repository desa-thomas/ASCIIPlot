/*
Functions for cartesian plane
*/

#include "cartesian-plane.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <stdarg.h> 

#define EPSILON 1e-6

Plane* init_plane()
{
    Plane* p = malloc(sizeof(Plane)); 
    if (!p) return NULL; 
    
    //Set centerX and centerY based on size of screen
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    //set width and height based on size of screen
    p->width = max_x - XPADDING*2;
    p->height = max_y - YPADDING-2;    

    //center and origin are center of window
    p->originX = p->width/2;
    p->originY = p->height/2;
    p->centerX = p->width/2;
    p->centerY = p->height/2; 

    p->scale = DEFAULT_SCALE;
    
    //init window
    p->win = init_window(p); 
    return p;
}

/* usage p->win = init_window(p)*/
WINDOW* init_window(Plane* p)
{
    WINDOW* win;
    win = newwin(p->height+1, p->width+1, YPADDING, XPADDING); 
    box(win, 0, 0); 
    return win; 
}

void clear_plane(Plane* p)
{
    wclear(p->win); 
}

void free_plane(Plane*p)
{
    free(p->win); 
    free(p);    
}

/* Clear before redrawing*/
void draw_plane(Plane*p)
{
    box(p->win, 0, 0); 

    if(p->originX > 1 && p->originX < p->width)
    {  
        for (int y = 1; y < p->height; y++)
        {
            if (y == p->originY) 
                mvwaddch(p->win, y, p->originX, '+');

            else mvwaddch(p->win, y, p->originX, '|');
        }
        
    }

    if(p->originY > 1 && p->originY < p->height)
    {
        for(int x = 1; x < p->width; x++)
        {
            if(x != p->originX) 
                mvwaddch(p->win, p->originY, x, '-'); 
        }
    }
}

void move_origin(Plane *p, Direction dir)
{
    switch (dir) 
    {
        case UP:
            p->originY -= MOVESPEED; 
            break; 
        
        case DOWN:
            p->originY += MOVESPEED; 
            break;

        case LEFT:  
            p->originX -= MOVESPEED; 
            break;
        
        case RIGHT:
            p->originX += MOVESPEED; 
            break; 
    }
}

void zoom (Plane* p, Zoom zoom)
{
    switch (zoom) 
    {   
        //decrease scale when zooming in
        case ZOOMIN: 
            if (p->scale > 10)
                p->scale -= ZOOMRATE_HIGH; 

            else if (p->scale > ZOOMRATE_MID)
                p->scale -= ZOOMRATE_MID;
          
            else if (p->scale > ZOOMRATE_LOW)
                p->scale -= ZOOMRATE_LOW; 

            else if (p->scale > ZOOMRATE_LOW_LOW){
                //Bug fix: 0.01 > 0.01 would be wrongly evaluated, add tolerance
                if(fabs(p->scale -ZOOMRATE_LOW_LOW) > EPSILON){
                    p->scale -= ZOOMRATE_LOW_LOW; }
                }
          
            break; 

        //increase scale when zooming out
        case ZOOMOUT:
            if (p->scale < ZOOMRATE_LOW){
                //check if something like 0.1 > 0.1 is happening
                if(fabs(ZOOMRATE_LOW) - p->scale > EPSILON){
                    p->scale += ZOOMRATE_LOW_LOW; 
                }
                else p->scale+= ZOOMRATE_LOW; 
            }
                

            else if(p->scale < ZOOMRATE_MID)
                p->scale += ZOOMRATE_LOW; 

            else if (p->scale < ZOOMRATE_HIGH)
                p->scale += ZOOMRATE_MID;

            else
                p->scale += ZOOMRATE_HIGH; 
            break; 
    }
}

/* Called when screen size changes, destroys and reconstructs window*/
void updateCenter(Plane *p)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    //set width and height based on size of screen
    p->width = max_x - XPADDING*2;
    p->height = max_y - YPADDING-2;    

    int centerX = p->width/2;
    int centerY = p->height/2; 

    /* Update origin based on the change in center*/
    int dx = centerX - p->centerX;
    int dy = centerY -p->centerY;
    p->originX += dx; 
    p->originY += dy;

    p->centerX = centerX;
    p->centerY = centerY;

    //Destroy and update window
    wclear(p->win); 
    delwin(p->win); 
    p->win = init_window(p); 
}

/*
TESTING FUNCTION AND MACROS FOR DRAWING A PARABOLA
*/
#define parabola_screenY(screenX, p) toScreenY(p, pow(toPlaneX(p, screenX), 2))
#define parabola_screenX_plus(screenY, p) toScreenX(p, sqrt(toPlaneY(p,screenY)))
#define parabola_screenX_minus(screenY, p) toScreenX(p, -sqrt(toPlaneY(p, screenY)))
void draw_parabola(Plane *p)
{
    //draw parabola red
    wattron(p->win, COLOR_PAIR(1));
    wattron(p->win, A_BOLD);  

    //Draw y points for each x point
    for (int screenX = 1; screenX < p->width; screenX++)
    {
        int screenY = parabola_screenY(screenX, p);

        if (screenY > 0 && screenY < p->height)
        {
            mvwaddch(p->win, screenY, screenX, '*'); 
        }

        //approximate the points between respective whole valued (x,y) points
        if (screenX + 1 <= p->width)
        {
            int nextScreenY =  parabola_screenY(screenX+1, p);
        

            //Goint from smaller y to bigger y (screen Y, so going down)
            for(int y = screenY; y< nextScreenY; y++)
            {   
                if (y > 0 && y < p->height)
                {
                    int inbetweenx_minus = parabola_screenX_minus(y, p);
                    mvwaddch(p->win, y, inbetweenx_minus, '*'); 
                    // mvwaddch(p->win, y, round(inbetweenx_minus -1), '*'); 

                }              
            }
            //going from bigger to smaller y (screen coordinates)
            for(int y = screenY; y > nextScreenY; y--)
            {
                if (y > 0 && y < p->height)
                {
                    int inbetweenx_plus = parabola_screenX_plus(y, p);
                    mvwaddch(p->win, y, inbetweenx_plus, '*'); 
                    // mvwaddch(p->win, y, round(inbetweenx_plus+ 1), '*'); 
                }
            }
        }
        
    }
    
    wattroff(p->win, COLOR_PAIR(1));
    wattroff(p->win, A_BOLD); 
}

void write_log (const char* msg, ...)
{
    va_list args; 
    va_start (args, msg); 

    FILE * file;
    file = fopen("out.log", "a"); 

    if (file != NULL)
    {
        vfprintf(file, msg, args);
        fprintf(file, "\n");
        fclose(file);
    }
    else 
    {
        perror("Failed to open log file"); 
    }
}
void clear_log()
{
    FILE * file;
    file = fopen("out.log", "w");
    if (file != NULL)
    {
        fprintf(file, "--DEBUGGING LOG--\n");
        fclose(file); 
    }
}