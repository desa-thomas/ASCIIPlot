typedef struct{
    //size of the width and height on screen
    int width;
    int height;

    //how many units does moving 1 dash from origin represent
    float scale;

    //coordinates of origin on screen
    int originX;
    int originY;

}Plane; 

/*
Create Cartesian plane
    width - width in ncurses window
    height - height in ncurses window
    scale - scale of the grid
    originX - xcoordinate of origin
    originY - ycoordinate of origin
*/
Plane* init_plane(int width, int height, float scale, int originX, int originY); 

/*
free up pointers
*/
void free_plane(Plane* plane);

/*
Draw plane using ncurses
*/
void draw_plane(Plane* p); 