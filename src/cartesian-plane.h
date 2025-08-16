#include <ncurses.h>
#include "fxp.h"

/*
Constants
*/
//Rate at which to change plane scale when zooming
#define ZOOMRATE_LOW_LOW 0.01f
#define ZOOMRATE_LOW 0.1f
#define ZOOMRATE_MID 1

#define ZOOMRATE_HIGH 2

//Rate at which to move origin 
#define MOVESPEED 1
//Default scale of graphs
#define DEFAULT_SCALE 0.1

#define XPADDING 2
#define YPADDING 10

/*
Cartesian plane struct prototypes
*/

//Cartesian plane struct
typedef struct
{
    //size of the width and height of the viewbox
    int width;
    int height;

    WINDOW* win; 

    //center of the viewbox on screen
    int centerX;
    int centerY;

    //Graph scale (i.e., units per '-')
    double scale;

    //coordinates of the graphs origin.
    //This differs from the center of the viewbox and is used to move around the graph
    int originX;
    int originY;

}Plane; 

typedef enum 
{
    ZOOMIN,
    ZOOMOUT
} Zoom; 

typedef enum 
{
    UP, 
    DOWN, 
    LEFT, 
    RIGHT
}Direction; 

//Transform screen coordinates to plane coordinates
//Note for the y-axis the distance formula is flipped, this is because
//y coordinates (in ncurses) are calculated by # of rows from top left
#define toPlaneY(p, screenY) ((p->originY - screenY)*p->scale)
#define toPlaneX(p, screenX) ((screenX - p->originX)*p->scale)

//Transform plane X and Y back to scren coordinates
#define toScreenY(p, planeY) (int) (round(p->originY - ((double)planeY/p->scale)))
#define toScreenX(p, planeX) (int) (round((double)planeX/p->scale + p->originX)) 

/* To apply function: f(x) you must:
1. transform screen coordinates to plane coordinates
2. evaulate function plane coordinates
3. transform plane coordinates back to their mapped screen coordinates*/

/*
Cartesian plane methods
*/

/*
Create a Cartesian plane. Values are calculated based on current screen size
*/
Plane* init_plane(); 

/*
Initialize plane window
*/
WINDOW* init_window(Plane* p); 

/*
Clear plane window
*/
void clear_plane(Plane* p); 

/*
free up pointers
*/
void free_plane(Plane* p);

/*
Draw the plane 
*/
void draw_plane(Plane* p); 

/*
Move origin in direction by speed of MOVESPEED 
*/
void move_origin(Plane* p, Direction dir); 
/*
Zoom into graph (i.e., increase or decrease scale by a rate of ZOOMRATE)
*/
void zoom(Plane* p, Zoom zoom);

/*
Destroy and update window
*/
void updateCenter(Plane*p);

/*
Test function, draws a parabola y=x^2
*/
void draw_parabola(Plane* p);

/*
 Graph a function on plane
 */
void graph_function(Plane* p, FOX* f); 
