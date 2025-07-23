/*
Functions for cartesian plane
*/

#include "cartesian-plane.h"
#include <stdlib.h>
#include <ncurses.h>

Plane* init_plane(int width, int height, float scale, int originX, int originY)
{
    Plane* p = malloc(sizeof(Plane)); 
    if (!p) return NULL; 

    p->width = width; 
    p->height = height;
    p->scale = scale; 
    p->originX = originX; 
    p->originY = originY; 

    return p;
}

void free_plane(Plane*p)
{
    free(p); 
}

void draw_plane(Plane*p)
{
    for(int x = p->originX - p->width/2; x < p->originX + p->width/2; x++)
    {
        if (x == p->originX) 
            mvaddch(p->originY, x, '+');
        else
            mvaddch(p->originY, x, '-');
    }

    for (int y = p->originY - p->height/2; y < p->originY + p->height/2; y++)
    {
        if (y != p->originY) mvaddch(y, p->originX, '|'); 
    }
}