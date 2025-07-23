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

Plane* init_plane(int width, int height, float scale, int originX, int originY)
{
    Plane* p = malloc(sizeof(Plane)); 
    if (!p) return NULL; 

    p->viewbox = getViewbox(originX, originY, width, height); 

    p->width = width; 
    p->height = height;
    p->scale = scale; 

    //Origin and center of box are initialized to the same value
    p->originX = originX; 
    p->originY = originY; 
    p->centerX = originX; 
    p->centerY = originY;

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