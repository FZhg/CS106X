/** 
 * File: ssview.cpp
 * -----------------
 * This file implements the ssview.h interface, which provides
 * the graphical view for the Stanford 1-2-3 program.
 *
 * Autumn 2007 Julie Zelenski (original implementation)
 * Autumn 2012 Jerry Cain (upgrade to use even newer libraries)
 * Autumn 2018 Jerry Cain (upgrade to use even *newer* libraries)
 */
 
#include "gobjects.h"
#include "ssview.h"
#include "ssutil.h"

#include <cctype>
#include <sstream>
#include <iomanip>
using namespace std;

/**
 * General implementation notes
 * ----------------------------
 * This file implements the spreadsheet graphical view for the
 * Stanford 1-2-3 program.  It makes straightforward use of the extended
 * graphics library to draw the grid, labels, cell contents, and
 * so forth.  The implementations of the individual member functions
 * are fairly simple, and you are feel free to change these if
 * you get inspired.  For descriptions of the behavior of the
 * exported functions, please see the interface file.
 */

static const double kRowHeight = 15;
static const double kColWidth = 96;
static const double kRowHeaderWidth = 22;
static const double kColHeaderHeight = 15;
static const double kHighlightPauseTime = 10;
static const char kEllipsisChar = char(201); // is ascii 133 on PC? Check...!

enum alignment { 
    LeftAligned, 
    Centered, 
    RightAligned 
};	// for text alignments

static const string kWindowTitle = "Stanford 1-2-3";

static const string kHeaderBackgroundColor = "#CCCCCC";
static const string kGridLineColor = "LightGray";
static const string kColLabelColor = "#000077";
static const string kRowLabelColor = "#007700";
static const string kTextColor = "#000000";
static const string kEmptyCellColor = "#FFFFFF";
static const string kHighlightColor = "#99FFFF";

static const string kPlainFont = "Courier-PLAIN-12";
static const string kBoldFont = "Courier-BOLD-12";

SSView::SSView(): GWindow(kRowHeaderWidth + kNumColsDisplayed * kColWidth,
                          kColHeaderHeight + kNumRowsDisplayed * kRowHeight) {
	setWindowTitle(kWindowTitle);
	cellHeight = (getHeight() - kColHeaderHeight)/kNumRowsDisplayed;
	cellWidth = (getWidth() - kRowHeaderWidth)/kNumColsDisplayed;
    displayEmptySpreadsheet();
}

void SSView::displayEmptySpreadsheet() {
    setRepaintImmediately(false);
    clear();
	labelAxes();
    for (int row = 1; row <= kNumRowsDisplayed; row++)
        for (char col = 'A'; col < 'A' + kNumColsDisplayed; col++)
            drawAndFillRect(rectForCell(col, row), kEmptyCellColor, 
                            kGridLineColor);
	repaint();
    setRepaintImmediately(true);
}

void SSView::displayCell(Cell *cell, bool highligthPause){
    displayCell(cell->getCellName(), cell->getStrData(), highligthPause);
}

void SSView::displayCell(const string& cellname, 
                         const string& txt, bool highlightPause) {
	drawOneCell(cellname, txt, RightAligned, 
                kEmptyCellColor, kHighlightColor, highlightPause);
}
  	
void SSView::drawOneCell(const string& cellname, const string& contents, 
                         int align, const string& bgColor, 
                         const string& highlightColor, bool highlightPause) {
	location loc;
	if (!stringToLocation(cellname, loc))
		error("displayCell called with invalid cell name " + cellname);
	rect r = rectForCell(loc.col, loc.row, 1);
	if (contents.empty()) // just erase background
		drawAndFillRect(r, bgColor, bgColor);
	else {
		double x, y;
		if (align == LeftAligned) x = r.x + 4;
		else if (align == Centered) x = r.x + cellWidth / 2;
        else x = r.x + cellWidth - 4;
		y = r.y + cellHeight - 4;
		string shortenContents = shortenStringToFit(contents, cellWidth - 4);
		if (highlightPause) {
			drawAndFillRect(r, highlightColor, highlightColor);
			drawAlignedText(shortenContents, kPlainFont, bgColor, x, y, align);
			pause(kHighlightPauseTime);
		}
		drawAndFillRect(r, bgColor, bgColor);
		drawAlignedText(shortenContents, kPlainFont, kTextColor, x, y, align);
	}
	repaint();
}

void SSView::labelAxes() {
	rect corner = {0, 0, kRowHeaderWidth, kColHeaderHeight};
	drawAndFillRect(corner, kHeaderBackgroundColor, "White");
	for (int row = 0; row < kNumRowsDisplayed; row++)
		drawRowLabel(row);
	for (int col = 0; col < kNumColsDisplayed; col++)
		drawColLabel(col);
}

void SSView::drawColLabel(int col) {
	rect r = {
        kRowHeaderWidth + col * cellWidth, 0, 
        cellWidth, kColHeaderHeight
    };
	drawAndFillRect(r, kHeaderBackgroundColor, "White");
	drawAlignedText(string(1, 'A' + col), kBoldFont, kColLabelColor,
                    r.x + 0.5 * cellWidth, r.y + 0.5 * kColHeaderHeight, 
                    Centered);
}

void SSView::drawRowLabel(int row) {
	rect r = {0, kColHeaderHeight + row * cellHeight, kRowHeaderWidth, cellHeight};
	drawAndFillRect(r, kHeaderBackgroundColor, "White");
	drawAlignedText(integerToString(row + 1), kBoldFont, kRowLabelColor,
                    r.x + 0.5 * kRowHeaderWidth, r.y + 0.5 * cellHeight, 
                    Centered);
}

SSView::rect SSView::rectForCell(char col, int row, double inset) {
	rect r;
	r.x = kRowHeaderWidth + (toupper(col) - 'A') * cellWidth + inset;
	r.y = kColHeaderHeight + (row - 1) * cellHeight + inset;
	r.width = cellWidth - 2 * inset;
	r.height = cellHeight - 2 * inset;
	return r;
}

void SSView::drawAndFillRect(const rect& r, const string& fillColor, 
                             const string& frameColor) {
	fillRect(r.x, r.y, r.width, r.height, fillColor);
	if (!frameColor.empty()) 
        drawRect(r.x, r.y, r.width, r.height, frameColor);
}

void SSView::drawRect(double x, double y, double w, double h, 
                      const string& color) {
    GRect rect(x, y, w, h);
    rect.setColor(color);
    draw(rect);
}

void SSView::fillRect(double x, double y, double w, double h, 
                      const string& color) {
    GRect rect(x, y, w, h);
    rect.setFilled(true);
    rect.setFillColor(color);
    draw(rect);
}

void SSView::drawAlignedText(const string& text, const string& font, const string& color,
                             double x, double y, int align) {
    GLabel label(text);
    label.setFont(font);
    label.setColor(color);
    if (align == Centered) { // centers both horiz and vert
        label.setLocation(x - label.getWidth()/2,
                          y + label.getFontAscent()/2);
    } else if (align == LeftAligned) {
        label.setLocation(x, y);
    } else if (align == RightAligned) {
        label.setLocation(x - label.getWidth(), y);
    }
    draw(label);
}

string SSView::shortenStringToFit(const string& s, double maxWidth) const {
    GLabel label(s);
    label.setFont(kPlainFont);
    if (label.getWidth() <= maxWidth) return s;

    string clone = s;
	while (true) {
        label.setLabel(clone + kEllipsisChar);
        if (label.getWidth() <= maxWidth) break;
        clone.replace(clone.length() - 1, 1, "");
    }
    return label.getLabel();
}

