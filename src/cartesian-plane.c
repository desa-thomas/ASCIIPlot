/*
Functions for cartesian plane
*/

#include "cartesian-plane.h"
#include "utilities.h"

#include <math.h>
#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>

#define EPSILON 1e-6

Plane *init_plane() {
  Plane *p = malloc(sizeof(Plane));
  if (!p)
    return NULL;

  // Set centerX and centerY based on size of screen
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  // set width and height based on size of screen
  p->width = max_x - XPADDING * 2;
  p->height = max_y - YPADDING - 2;

  // center and origin are center of window
  p->originX = p->width / 2;
  p->originY = p->height / 2;
  p->centerX = p->width / 2;
  p->centerY = p->height / 2;

  p->scale = DEFAULT_SCALE;

  // init window
  p->win = init_window(p);
  return p;
}

/* usage p->win = init_window(p)*/
WINDOW *init_window(Plane *p) {
  WINDOW *win;
  win = newwin(p->height + 1, p->width + 1, YPADDING, XPADDING);
  box(win, 0, 0);
  return win;
}

void clear_plane(Plane *p) { wclear(p->win); }

void free_plane(Plane *p) {
  free(p->win);
  free(p);
}

/* Clear before redrawing*/
void draw_plane(Plane *p) {
  box(p->win, 0, 0);

  // If y axis is visible
  if (p->originX > 1 && p->originX < p->width) {
    for (int y = 1; y < p->height; y++) {
      if (y == p->originY)
        mvwaddch(p->win, y, p->originX, '+');

      else
        mvwaddch(p->win, y, p->originX, '|');
    }
  }

  // if x axis is visible draw it
  if (p->originY > 1 && p->originY < p->height) {
    for (int x = 1; x < p->width; x++) {
      if (x != p->originX) {
        const double planex = toPlaneX(p, x);
        const double epsilon = 1e-6;          // tolerance threshold
        double mod = fmod(fabs(planex), 2.0); // remainder when divided by 2\fd
                                              //
        if (fabs(mod) < epsilon || fabs(mod - 2.0) < epsilon) {
          mvwaddch(p->win, p->originY, x, '|');
        } else
          mvwaddch(p->win, p->originY, x, '-');
      }
    }
  }
}

void move_origin(Plane *p, Direction dir) {
  switch (dir) {
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

void zoom(Plane *p, Zoom zoom) {
  switch (zoom) {
  // decrease scale when zooming in
  case ZOOMIN:
    if (p->scale > 10 && !double_equal(p->scale, 10))
      p->scale -= ZOOMRATE_HIGH;

    else if (p->scale > ZOOMRATE_MID && !double_equal(p->scale, ZOOMRATE_MID))
      p->scale -= ZOOMRATE_MID;

    else if (p->scale > ZOOMRATE_LOW && !double_equal(p->scale, ZOOMRATE_LOW))
      p->scale -= ZOOMRATE_LOW;

    else if (p->scale > ZOOMRATE_LOW_LOW &&
             !double_equal(p->scale, ZOOMRATE_LOW_LOW))
      p->scale -= ZOOMRATE_LOW_LOW;

    break;

  // increase scale when zooming out
  case ZOOMOUT:
    if (p->scale < ZOOMRATE_LOW) {
      // check if something like 0.1 > 0.1 is happening
      if (fabs(ZOOMRATE_LOW) - p->scale > EPSILON) {
        p->scale += ZOOMRATE_LOW_LOW;
      } else
        p->scale += ZOOMRATE_LOW;
    }

    else if (p->scale < ZOOMRATE_MID)
      p->scale += ZOOMRATE_LOW;

    else if (p->scale < ZOOMRATE_HIGH)
      p->scale += ZOOMRATE_MID;

    else
      p->scale += ZOOMRATE_HIGH;
    break;
  }
}

/* Called when screen size changes, destroys and reconstructs window*/
void updateCenter(Plane *p) {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  // set width and height based on size of screen
  p->width = max_x - XPADDING * 2;
  p->height = max_y - YPADDING - 2;

  int centerX = p->width / 2;
  int centerY = p->height / 2;

  /* Update origin based on the change in center*/
  int dx = centerX - p->centerX;
  int dy = centerY - p->centerY;
  p->originX += dx;
  p->originY += dy;

  p->centerX = centerX;
  p->centerY = centerY;

  // Destroy and update window
  wclear(p->win);
  delwin(p->win);
  p->win = init_window(p);
}

/*
TESTING FUNCTION AND MACROS FOR DRAWING A PARABOLA
*/
#define parabola_screenY(screenX, p) toScreenY(p, pow(toPlaneX(p, screenX), 2))
#define parabola_screenX_plus(screenY, p)                                      \
  toScreenX(p, sqrt(toPlaneY(p, screenY)))
#define parabola_screenX_minus(screenY, p)                                     \
  toScreenX(p, -sqrt(toPlaneY(p, screenY)))
void draw_parabola(Plane *p) {
  // draw parabola red
  wattron(p->win, COLOR_PAIR(1));
  wattron(p->win, A_BOLD);

  // Draw y points for each x point
  for (int screenX = 1; screenX < p->width; screenX++) {
    int screenY = parabola_screenY(screenX, p);

    if (screenY > 0 && screenY < p->height) {
      mvwaddch(p->win, screenY, screenX, '*');
    }

    // approximate the points between respective whole valued (x,y) points
    if (screenX + 1 <= p->width) {
      int nextScreenY = parabola_screenY(screenX + 1, p);

      // Goint from smaller y to bigger y (screen Y, so going down)
      for (int y = screenY; y < nextScreenY; y++) {
        if (y > 0 && y < p->height) {
          int inbetweenx_minus = parabola_screenX_minus(y, p);
          mvwaddch(p->win, y, inbetweenx_minus, '*');
          // mvwaddch(p->win, y, round(inbetweenx_minus -1), '*');
        }
      }
      // going from bigger to smaller y (screen coordinates)
      for (int y = screenY; y > nextScreenY; y--) {
        if (y > 0 && y < p->height) {
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

void graph_function(Plane *p, FOX *f) {

  wattron(p->win, COLOR_PAIR(1));
  wattron(p->win, A_BOLD);

  double prev_x = NAN;
  double prev_y = NAN;
  int prev_screen_y = 0;

  Bool wasinf = False;

  // Draw y points for each x point on screen
  for (int screenX = 1; screenX < p->width; screenX++) {
    const double x =
        toPlaneX(p, screenX); // convert screen coord to plane coord
    double y = evaluate_f(f, x);

    if (!isnan(y)) {

      // to simulate infinity, assign y to largest/smallest coordinate
      if (y == INFINITY) {
        y = toPlaneY(p, 0);
        wasinf = True;
      } else if (y == -INFINITY) {
        y = toPlaneY(p, p->height);
        wasinf = True;
      } else {
        wasinf = False;
      }

      const int screenY = toScreenY(p, y);

      // if y coord is in window
      if ((screenY > 0 && screenY < p->height) || (prev_screen_y > 0 && prev_screen_y < p->height)) {

        if(screenY > 0 && screenY < p->height)
        mvwaddch(p->win, screenY, screenX, '*');

      // interpolate y values between two subsequent screenX values
      if (!isnan(prev_x) && !wasinf) {
        for (int inbetweenScreenY = prev_screen_y + 1;
             inbetweenScreenY < screenY; inbetweenScreenY++) {
          const double inbetweenPlaneY = toPlaneY(p, inbetweenScreenY);
          const double interpolatedPlaneX =
              linear_interpolation_x(prev_x, prev_y, x, y, inbetweenPlaneY);
          const int interpolatedScreenX = toScreenX(p, interpolatedPlaneX);

          if (inbetweenScreenY > 0 && inbetweenScreenY < p->height) {

            mvwaddch(p->win, inbetweenScreenY, interpolatedScreenX, '*');
          }
        }
        for (int inbetweenScreenY = prev_screen_y - 1;
             inbetweenScreenY > screenY; inbetweenScreenY--) {

          const double inbetweenPlaneY = toPlaneY(p, inbetweenScreenY);
          const double interpolatedPlaneX =
              linear_interpolation_x(prev_x, prev_y, x, y, inbetweenPlaneY);
          const int interpolatedScreenX = toScreenX(p, interpolatedPlaneX);

          if (inbetweenScreenY > 0 && inbetweenScreenY < p->height) {

            mvwaddch(p->win, inbetweenScreenY, interpolatedScreenX, '*');
          }
        }
      }}
      if (wasinf == 1) {
        prev_x = NAN;
        prev_y = NAN;
        prev_screen_y = 0;
      } else {
        prev_x = x;
        prev_y = y;
        prev_screen_y = screenY;
      }
    }
  }

  wattroff(p->win, COLOR_PAIR(1));
  wattron(p->win, A_BOLD);
}
