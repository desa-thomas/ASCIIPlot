/*
Desmos in your terminal, you absolutely don't need it
*/

#include <locale.h>
#include <ncurses.h>
#include <stdio.h>

#include "cartesian-plane.h"
#include "utilities.h"

#define MAX_FUNCTIONS 3
#define MAX_FUNCTION_STRING_LEN 500
typedef enum { FX, GX, HX } INPUT_FUNCTION;

void draw_menu();
void create_input_windows();
void draw_input_windows();
void initialize_colors();
void write_char_to_input(char ch);
void del_char();
void refresh_plane();
void handle_input(int ch);
void draw_functions();

WINDOW *input_windows[MAX_FUNCTIONS] = {NULL};
char *func_names[MAX_FUNCTIONS] = {"f(x)", "g(x)", "h(x)"};
FOX *function_objs[MAX_FUNCTIONS] = {NULL};

char function_strings[MAX_FUNCTIONS][MAX_FUNCTION_STRING_LEN] = {0};
int function_strings_lens[MAX_FUNCTIONS] = {0};

int cursor_pos[MAX_FUNCTIONS] = {0};

INPUT_FUNCTION current_function = FX;

Bool refresh_graph = False;
Bool refresh_inputs = False;

Plane *plane = NULL;

int main() {
  clear_log(); // clear debugging log

  // Enable UTF-8 Locale (for title art)
  setlocale(LC_ALL, "");
  /* Initialize ncurses */
  initscr();            // init screen
  raw();                // enable raw mode (how to handle ctrl c)
  noecho();             // disable echoing
  keypad(stdscr, true); // enable keypad
  curs_set(0);          // make cursor invisible
  start_color();        // enable colors
  // nodelay(stdscr, true); //make getch non-blocking

  use_default_colors(); 
  /* Initialize colors */
  initialize_colors();

  /* Initalize cartesian plane */
  plane = init_plane();

  int max_x, max_y, prev_max_x, prev_max_y;
  int ch;

  draw_plane(plane);

  refresh();
  wrefresh(plane->win);

  create_input_windows();
  draw_menu();

  while (true) {
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(0, max_x - XPADDING - 12, "SCALE: %.2f", plane->scale);

    if (refresh_graph) {
      refresh_plane();
    }
    refresh_graph = True;

    if (refresh_inputs)
      draw_input_windows();

    int x, y;
    getyx(input_windows[current_function], y, x);
    wmove(input_windows[current_function], y, x);

    wrefresh(input_windows[current_function]);
    refresh_inputs = False;

    /* If ctrl-c is pressed, quit*/
    ch = getch();
    if (ch == 3) {
      break;
    }

    else {
      handle_input(ch);
    }

    /* If screen size changes*/
    prev_max_x = max_x;
    prev_max_y = max_y;
    getmaxyx(stdscr, max_y, max_x);
    if (prev_max_x != max_x || prev_max_y != max_y) {
      clear();
      draw_menu();
      updateCenter(plane);
      refresh();
      refresh_plane();
      create_input_windows();
    }
  }

  endwin();
  return 0;
}

// Draw menu on main window
void draw_menu() {
  int max_x, title_row;
  max_x = getmaxx(stdscr);
  /* Title card setup*/
  char *title_line1 = "░█▀█░█▀▀░█▀▀░▀█▀░▀█▀░█▀█░█░░░█▀█░▀█▀";
  char *title_line2 = "░█▀█░▀▀█░█░░░░█░░░█░░█▀▀░█░░░█░█░░█░";
  char *title_line3 = "░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░░░▀▀▀░▀▀▀░░▀░";

  attron(COLOR_PAIR(2));
  attron(A_BOLD);

  attron(A_BOLD);
  attroff(COLOR_PAIR(2));

  /* TITLE CARD*/
  title_row = max_x / 2 - 19;
  mvprintw(1, title_row, "%s", title_line1);
  mvprintw(2, title_row, "%s", title_line2);
  mvprintw(3, title_row, "%s", title_line3);
}

void initialize_colors() {

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
}

void create_input_windows() {
  const int max_x = getmaxx(stdscr);

  for (int i = 0; i < MAX_FUNCTIONS; i++) {
    input_windows[i] = newwin(1, max_x - XPADDING * 2, 5 + i, XPADDING);
  }
  draw_input_windows();
}

void draw_input_windows() {

  for (int i = 0; i < MAX_FUNCTIONS; i++) {

    wclear(input_windows[i]);
    wattron(input_windows[i], COLOR_PAIR(i + 1));

    if (i == (int)current_function) {
      wattron(input_windows[i], A_BOLD);
      mvwprintw(input_windows[i], 0, 0, " >  %s = ", func_names[i]);
      wattroff(input_windows[i], A_BOLD);
    } else {

      mvwprintw(input_windows[i], 0, 0, "[%d] %s = ", i + 1, func_names[i]);
    }
    wattroff(input_windows[i], COLOR_PAIR(i + 1));

    mvwprintw(input_windows[i], 0, 11, "%s", function_strings[i]);

    // draw cursor after drawing function
    if (i == (int)current_function) {
      wattron(input_windows[i], COLOR_PAIR(i + 1));
      const int x = 11 + cursor_pos[current_function];
      mvwaddch(input_windows[current_function], 0, x, '_');
      wattroff(input_windows[i], COLOR_PAIR(i + 1));
    }
    wrefresh(input_windows[i]);
  }
}

void refresh_plane() {
  /* Draw plane and graphs*/
  clear_plane(plane);
  draw_plane(plane);
  draw_functions();
  wrefresh(plane->win);
}

void handle_input(const int ch) {

  /* Arrow keys for moving around graph*/
  if (ch == KEY_UP)
    move_origin(plane, DOWN);
  else if (ch == KEY_DOWN)
    move_origin(plane, UP);
  else if (ch == KEY_LEFT)
    move_origin(plane, RIGHT);
  else if (ch == KEY_RIGHT)
    move_origin(plane, LEFT);

  /* Page up and Page down for zooming*/
  else if (ch == KEY_NPAGE)
    zoom(plane, ZOOMOUT);

  else if (ch == KEY_PPAGE)
    zoom(plane, ZOOMIN);

  else if (ch == KEY_BACKSPACE) {

    const int x = getcurx(input_windows[0]);
    if (x > 11) {
      wmove(input_windows[current_function], 0,
            cursor_pos[current_function] + 11 - 1);
      wdelch(input_windows[current_function]);

      // update string buffer
      if (function_strings_lens[current_function] > 0) {

        if (cursor_pos[current_function] ==
            function_strings_lens[current_function])
          cursor_pos[current_function] -= 1;

        function_strings_lens[current_function] -= 1;

        const int endpos = function_strings_lens[current_function];
        function_strings[current_function][endpos] = '\0';
      }
      refresh_inputs = True;
    }
    refresh_graph = False;
  }

  // enter key
  else if (ch == KEY_ENTER || ch == '\n') {
    FOX *f = initfunc(function_strings[current_function]);
    function_objs[current_function] = f;
    if (!f) {
      wattron(input_windows[current_function], COLOR_PAIR(3));
      mvwprintw(input_windows[current_function], 0,
                12 + function_strings_lens[current_function],
                "Error Parsing function!");
      wattroff(input_windows[current_function], COLOR_PAIR(3));
    }
  }

  // listen for alt key
  else if (ch == 27) {
    const int next = getch();

    switch (next) {
    case '1':
      current_function = FX;
      break;
    case '2':
      current_function = GX;
      break;
    case '3':
      current_function = HX;
      break;

    case 'j':
    case 'n':
      if (current_function < HX)
        current_function += 1;
      break;
    case 'k':
    case 'N':
      if (current_function > FX)
        current_function -= 1;
      break;

    case 'h':
      if (cursor_pos[current_function] > 0)
        cursor_pos[current_function] -= 1;
      break;

    case 'l':
      if (cursor_pos[current_function] <
          function_strings_lens[current_function])
        cursor_pos[current_function] += 1;
    }
    refresh_inputs = True;
  }

  // normal input
  else {
    // check for overflow
    if (function_strings_lens[current_function] < MAX_FUNCTION_STRING_LEN) {

      mvwaddch(input_windows[current_function], 0, cursor_pos[current_function],
               ch);

      if (cursor_pos[current_function] ==
          function_strings_lens[current_function]) {
        // update string buffer
        function_strings_lens[current_function] += 1;
        const int endpos = function_strings_lens[current_function];

        function_strings[current_function][endpos - 1] = ch;
        function_strings[current_function][endpos] = '\0';
      } else {
        function_strings[current_function][cursor_pos[current_function]] = ch;
      }
      cursor_pos[current_function] += 1;
    }
    refresh_inputs = True;

    refresh_graph = False;
  }
}

void draw_functions() {

  for (int i = 0; i < MAX_FUNCTIONS; i++) {
    if (function_objs[i]) {
      wattron(plane->win, COLOR_PAIR(i + 1));
      graph_function(plane, function_objs[i]);
      wattroff(plane->win, COLOR_PAIR(i + 1));
    }
  }
}
