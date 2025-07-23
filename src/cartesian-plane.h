/*
Cartesian plane struct prototypes
*/

//Viewbox of plane, values calculated based on width, height and center
typedef struct
{
    int xStart;
    int xEnd; 
    int yStart; 
    int yEnd; 
} Viewbox; 

//Cartesian plane struct
typedef struct
{
    //size of the width and height of the viewbox
    int width;
    int height;

    //center of the viewbox on screen
    int centerX;
    int centerY;

    //Viewbox's corners
    Viewbox* viewbox; 

    //Graph scale (i.e., units per '-')
    float scale;

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

/*
Constants
*/

//Rate at which to change plane scale when zooming
#define ZOOMRATE 5
//Rate at which to move origin 
#define MOVESPEED 1
//Default scale of graphs
#define DEFAULT_SCALE 5

//get viewbox values based on center and size of box
Viewbox* getViewbox(int centerX, int centerY, int width, int height);

/*
Cartesian plane methods
*/

/*
Create a Cartesian plane. Values are calculated based on current screen size
*/
Plane* init_plane(); 

/*
free up pointers
*/
void free_plane(Plane* p);

/*
Draw the viewbox
*/
void draw_box(Plane* p); 
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
Update center of viewbox, called when screen is resized
*/
void updateCenter(Plane*p);
/*
Update values of viewbox struct, called by updateCenter
*/
void updateViewbox(Plane*p); 