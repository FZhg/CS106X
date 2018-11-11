/**
 * File: ssview.h
 * --------------
 * This file defines the interface for SSView class used in the
 * Stanford 1-2-3 spreadsheet program.
 */

#pragma once
#include <string>
#include "gwindow.h"

/*********************************************************
 * NOTE: DO NOT EDIT THIS CLASS UNLESS DOING EXTENSIONS. *
 *********************************************************/

/**
 * Class constants: NumRowsDisplayed, NumColsDisplayed
 * Usage: int count = NumRowsDisplayed;
 * --------------------------------------------
 * Constants that control the number of visible rows/cols
 * in the graphics window.
 */

static const int kNumRowsDisplayed = 35;
static const int kNumColsDisplayed = 20;

/**
 * Class: SSView
 * --------------
 * This class handles the display of a spreadsheet in the graphics
 * window.  It exports two public member functions, one to display the
 * spreadsheet grid/labels/background, and another to display the 
 * contents for a given cell.  The member functions are intended to
 * be invoked by the model when cells are updated.
 */

class SSView : private GWindow {
public:
  
/**
 * Constructor: SSView
 * Usage: SSView view;
 * -------------------
 * The constructor initializes the graphics window, configures
 * the view object, and displays an empty spreadsheet.
 */

    SSView();

/** 
 * Member function: displayEmptySpreadsheet
 * Usage: view.displayEmptySpreadsheet();
 * -------------------------------------
 * This member function draws an empty spreadsheet. It will erase any
 * previous contents by filling the background. It then redraws the
 * grid and row/col labels.  All cells are drawn empty.
 */
    
  	void displayEmptySpreadsheet();
  	
/**
 * Member function: displayCell
 * Usage: view.displayCell(name, contents);
 * ----------------------------------------
 * This member function draws the contents for a given cell.
 * Cell name format is column letter followed by row number, e.g. "A7".  
 * Columns are lettered starting from 'A'.  Rows are numbered starting from 1. 
 * Note that rows are not zero-based (typical users don't count from zero!)
 * An error is raised if cellname is invalid.  The string is drawn
 * right-aligned within the cell. If the string is too long, it is
 * truncated to fit the cell. If the highlight flag is true, it will highlight 
 * the changed cell by drawing in inverse, pausing briefly, and then re-drawing 
 * normally. 
 */
  	
    void displayCell(const std::string& cellname, const std::string& txt, 
                     bool highlightPause = false);
	
private:  
  	struct rect {
        double x, y, width, height;
    };
  	rect rectForCell(char col, int row, double inset = 0);
	void drawOneCell(const std::string& cellname, const std::string& s, 
                     int align, const std::string& bgColor, 
                     const std::string& highlightColor, bool highlightPause);
    void drawRect(double x, double y, double w, double h, 
                  const std::string& color);
  	void fillRect(const rect& r, const std::string& color);
    void fillRect(double x, double y, double w, double h, 
                  const std::string& color);
  	void drawAndFillRect(const rect& r, const std::string& fillColor, 
                         const std::string& frameColor);
    void drawAlignedText(const std::string& text, const std::string& font, const std::string& color,
                         double x, double y, int align);
    std::string shortenStringToFit(const std::string& s, double maxWidth) const;
  	void drawRowLabel(int row);
  	void drawColLabel(int col);
  	void labelAxes();
  	
  	double cellWidth, cellHeight;
};

