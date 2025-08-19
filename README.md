░█▀█░█▀▀░█▀▀░▀█▀░▀█▀░█▀█░█░░░█▀█░▀█▀
░█▀█░▀▀█░█░░░░█░░░█░░█▀▀░█░░░█░█░░█░
░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░░░▀▀▀░▀▀▀░░▀░

Graphing calculator for your terminal!

![Example screenshot](screenshots/asciiplot_example.png)

## About
ASCII Plot is a terminal-based function plotter built in C using ncurses. It features a custom-built mathematical expression parser designed specifically for this project. The parser constructs an expression tree to efficiently evaluate functions at any given x-coordinate.

To render a graph, the program iterates through the visible x-values in the terminal window, evaluates the function at each point, and plots the corresponding y-values. Since the terminal is limited to displaying ASCII characters in a grid, the graph simulates curves by linearly interpolating between adjacent points to create a smoother visual approximation.

Hope you enjoy!

## Usage
You need the ncurses library and Make installed.
- Clone this repository
- `cd` into the repo
- run `make`
- run `./main.exe` to start the application

## Controls
- `<M-[1,2,3]>` select function input (1, 2, 3). 
- `<M-n>` or `<M-j>` next function input. 
- `<M-N>` or `<M-k>` prev function input.
- Enter key on selected function to graph it. 
- Arrow keys to move around graph. 
- pg up and pg down to zoom in and out of the plane. 

## Gallery

![example 1](screenshots/ex1.png)
