/*
Functions for cartesian plane
*/

#include "cartesian-plane.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <stdarg.h> 

#define EPSILON 1e-6

//create viewbox of plane based on origin and size
Viewbox* getViewbox(int centerX, int centerY, int width, int height)
{
    Viewbox* viewbox = malloc(sizeof(Viewbox));
    int xStart = centerX - width/2; 
    int xEnd = centerX + width/2; 
    int yStart = centerY - height/2; 
    int yEnd = centerY + height/2; 

    viewbox->xEnd = xEnd; 
    viewbox->xStart = xStart; 
    viewbox->yStart = yStart; 
    viewbox->yEnd = yEnd; 

    return viewbox; 
}


Plane* init_plane()
{
    Plane* p = malloc(sizeof(Plane)); 
    if (!p) return NULL; 
    
    //Set centerX and centerY based on size of screen
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    p->centerX = max_x/2;
    p->centerY = max_y/2;
    p->originX = max_x/2;
    p->originY = max_y/2; 
    //set width and height based on size of screen
    p->width = max_x - XPADDING;
    p->height = max_y - YPADDING;    

    p->viewbox = getViewbox(p->originX, p->originY, p->width, p->height); 

    p->scale = DEFAULT_SCALE;

    return p;
}

void free_plane(Plane*p)
{
    free(p->viewbox); 
    free(p); 
}

void draw_box(Plane* p)
{
    Viewbox* box = p->viewbox; 

    for (int x = box->xStart; x <= box->xEnd; x++)
    {
        mvaddch(box->yStart, x, '-' |A_BOLD); 
        mvaddch(box->yEnd, x, '-'|A_BOLD); 
    }

    for (int y = box->yStart; y <= box->yEnd; y++)
    {
        mvaddch(y, box->xStart,'|' | A_BOLD); 
        mvaddch(y, box->xEnd, '|' | A_BOLD); 
    }

    
}

void draw_plane(Plane*p)
{
    int xStart = p->viewbox->xStart;
    int xEnd = p->viewbox->xEnd; 
    int yStart = p->viewbox->yStart; 
    int yEnd = p->viewbox->yEnd; 

    //only draw axes that are in viewbox

    //If y axis is in view box (i.e., originX is there) draw it
    if(p->originX > xStart && p->originX < xEnd)
    {  
        for (int y = yStart +1 ; y < yEnd; y++)
        {
            if (y == p->originY) mvaddch(y, p->originX, '+');
            else mvaddch(y, p->originX, '|');
        }
        
    }

    if(p->originY > yStart && p->originY < yEnd)
    {
        for(int x = xStart+1; x < xEnd; x++)
        {
            if(x != p->originX) mvaddch(p->originY, x, '-'); 
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
            if (p->scale <= ZOOMRATE_LOW)
                p->scale += ZOOMRATE_LOW_LOW; 

            else if(p->scale <= ZOOMRATE_MID)
                p->scale += ZOOMRATE_LOW; 

            else if (p->scale <= ZOOMRATE_HIGH)
                p->scale += ZOOMRATE_MID;

            else
                p->scale += ZOOMRATE_HIGH; 
            break; 
    }
}

void updateCenter(Plane *p)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int centerY = max_y /2;
    int centerX = max_x /2; 

    p->width = max_x - XPADDING;
    p->height = max_y - YPADDING;

    /* Update origin based on the change in center*/
    int dx = centerX - p->centerX;
    int dy = centerY -p->centerY;
    p->originX += dx; 
    p->originY += dy;

    p->centerX = centerX;
    p->centerY = centerY;

    //update viewbox size
    updateViewbox(p); 
}

void updateViewbox(Plane *p)
{
    int xStart = p->centerX - p->width/2; 
    int xEnd = p->centerX + p->width/2; 
    int yStart = p->centerY - p->height/2; 
    int yEnd = p->centerY + p->height/2; 

    Viewbox* viewbox = p->viewbox; 
    viewbox->xEnd = xEnd; 
    viewbox->xStart = xStart; 
    viewbox->yStart = yStart; 
    viewbox->yEnd = yEnd; 
}

/*
TESTING FUNCTION AND MACROS FOR DRAWING A PARABOLA
*/
#define parabola_screenY(screenX, p) screenY(p, pow(planeX(p, screenX), 2))
#define parabola_screenX_plus(screenY, p) screenX(p, sqrt(planeY(p,screenY)))
#define parabola_screenX_minus(screenY, p) screenX(p, -sqrt(planeY(p, screenY)))
void draw_parabola(Plane *p)
{
    //draw parabola red
    attron(COLOR_PAIR(1)); 
    for(int x = p->viewbox->xStart; x < p->viewbox->xEnd; x++)
    {
        int screenY = parabola_screenY(x, p);

        //If transformed y point lies in viewbox
        if (screenY > p->viewbox->yStart && screenY < p->viewbox->yEnd)
        {
            mvaddch(screenY, x, '*' |A_BOLD); 
        }

        //fill curve between exact y points
        int nextScreenY = parabola_screenY(x+1, p);
        for (int y = screenY; y< nextScreenY; y++)
        {
            if(y > p-> viewbox->yStart && y < p->viewbox->yEnd)
            {
            int inbetweenX_plus, inbetweenX_minus;
            inbetweenX_plus = round(parabola_screenX_plus(y, p));
            inbetweenX_minus = round(parabola_screenX_minus(y, p)); 

            mvaddch(y, inbetweenX_plus, '*' |A_BOLD); 
            /* TODO add and extra '*' based on derivative values 
            i.e., to the left if derivative is neg, right if positive
            and only if it is a certain steepness ... i.e., < 1*/
            mvaddch(y, inbetweenX_plus +1 , '*'|A_BOLD);
            mvaddch(y, inbetweenX_minus-1, '*'|A_BOLD);
            mvaddch(y, inbetweenX_minus, '*'|A_BOLD); 

            }
        }

    }
    attroff(COLOR_PAIR(1));
}

void write_log (const char* msg, ...)
{
    va_list args; 
    va_start (args, msg); 

    FILE * file;
    file = fopen("out.log", "w"); 

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