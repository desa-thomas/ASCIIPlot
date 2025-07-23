/*
Functions for cartesian plane
*/

#include "cartesian-plane.h"
#include <stdlib.h>
#include <ncurses.h>

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
    p->width = max_x - 20;
    p->height = max_y -10;    

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
            if((p->scale - ZOOMRATE) < 1) p->scale = 1; 
            p->scale -= ZOOMRATE; 
            break;    

        //increase scale when zooming out
        case ZOOMOUT:
            p->scale += ZOOMRATE; 
            break;
    }
}

void updateCenter(Plane *p)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int centerY = max_y /2;
    int centerX = max_x /2; 

    p->width = max_x - 20;
    p->height = max_y -10;

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