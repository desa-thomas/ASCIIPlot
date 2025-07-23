/*
Cartesian plane struct prototype
*/

//Viewbox of plane
typedef struct
{
    int xStart;
    int xEnd; 
    int yStart; 
    int yEnd; 
} Viewbox; 

typedef struct
{
    //size of the width and height of box on screen
    int width;
    int height;

    //center of box on screen
    int centerX;
    int centerY;
    Viewbox* viewbox; 

    //how many units does moving 1 dash from origin represent
    float scale;

    //coordinates of origin on screen
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

/*
Constants
*/

//Rate at which to change plane scale when zooming
#define ZOOMRATE 5
//Rate at which to move origin 
#define MOVESPEED 1
#define DEFAULT_SCALE 5

Viewbox* getViewbox(int centerX, int centerY, int width, int height);
/*
Cartesian plane methods
*/

/*
Create Cartesian plane
    width - width in ncurses window
    height - height in ncurses window
    scale - scale of the grid
    originX - xcoordinate of origin
    originY - ycoordinate of origin
*/
Plane* init_plane(); 

/*
free up pointers
*/
void free_plane(Plane* p);

/*
Draw viewbox
*/
void draw_box(Plane* p); 
/*
Draw plane using ncurses
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
*/
void updateCenter(Plane*p);
void updateViewbox(Plane*p); 