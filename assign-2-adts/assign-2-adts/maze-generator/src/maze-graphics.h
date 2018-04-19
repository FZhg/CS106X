/**
 * File: maze-graphics.h
 * ---------------------
 * Exports the view class that displays the generation
 * of a maze.
 */

#ifndef _maze_graphics_
#define _maze_graphics_

#include <string>
#include "gwindow.h"
#include "maze-types.h"

class MazeGeneratorView : private GWindow {
public:
    
/**
 * Method: constructor
 * -------------------
 * Presents a clear graphics window so that calls to 
 * drawBorder and drawWall can be made to help animate
 * the construction of a simple maze.
 */

    MazeGeneratorView();

/**
 * Method: destructor
 * ------------------
 * Closes and otherwise destroys the MazeGeneratorView.
 */
    ~MazeGeneratorView();
    
/**
 * Method: setDimension
 * --------------------
 * Sets the dimension of the maze, which is understood to be square, to
 * be that provided.
 */
    void setDimension(int dimension);

/**
 * Method: drawBorder
 * ------------------
 * Draws a border around the confines of the maze.
 */
    void drawBorder();
    
/**
 * Method: drawWall
 * ----------------
 * Draws the wall presented via the only argument.
 */
    void drawWall(const wall& w);

/**
 * Method: removeWall
 * ------------------
 * Removes a previously drawn wall, which is the
 * wall provided.
 */
    void removeWall(const wall& w);
    
/**
 * Method: repaint
 * ---------------
 * Forces the accumulation of all draw and remove operations to actually show
 * up in the graphics window.
 */
    void repaint() { GWindow::repaint(); } // inlined for convenience

private:
    int dimension;
    double wallLength;
    double ulx;
    double uly;
    
    void drawWall(const wall& w, const std::string& color, double inset);
    void drawColoredLine(double startx, double starty, double endx, double endy, const std::string& color);
};

#endif
