/*
 * File: life-graphics.cpp
 * ----------------------------------------------------------
 * Implementation of the Life graphics support routines.  We have some
 * internal private helpers that do calculations for cell-size, 
 * colors, and "aging" the cells using shading. You are
 * welcome to read over this code, but you will not need to edit it unless 
 * you're attempting some sort of fancy extension.
 * 
 * This is based on a previous implementation by Julie Zelenski.
 */

#include <sstream>  // for ostringstream
#include <iomanip>  // for setw, setfill
#include <ios>      // for hex stream manipulator
using namespace std;
#include "random.h" // for randomInteger
#include "strlib.h" // for integerToString
#include "error.h"  // for error

#include "life-constants.h"
#include "life-graphics.h"
const string LifeDisplay::kDefaultWindowTitle("Game of Life");

LifeDisplay::LifeDisplay() : window(kDisplayWidth, kDisplayHeight) {
    initializeColors();
    window.setVisible(true);
    window.setWindowTitle(kDefaultWindowTitle);
    window.setRepaintImmediately(true);
}

LifeDisplay::~LifeDisplay() {
    window.close();
}

void LifeDisplay::setDimensions(int numRows, int numColumns) {
    if (numRows <= 0 || numColumns <= 0) {
        error("LifeDisplay::setDimensions number of rows and columns must both be positive!");
    }
    
    this->numRows = numRows;
    this->numColumns = numColumns;
    ages.resize(numRows, numColumns);
    computeGeometry();
    window.clear();
    window.setColor("White");
    window.drawLine(0, 0, 100, 100);
    window.fillRect(0, 0, kDisplayWidth, kDisplayHeight);
    window.setColor("Black");
    window.drawRect(upperLeftX, upperLeftY,
                    numColumns * cellDiameter + 1, numRows * cellDiameter + 1);
}

void LifeDisplay::setTitle(const string& title) {
    window.setWindowTitle(title);
    windowTitle = title;
}

void LifeDisplay::drawCellAt(int row, int column, int age) {
    if (!coordinateInRange(row, column)) {
        error(string(__FUNCTION__) + " asked to draw location (" +
              integerToString(row) + ", " + integerToString(column) +
              ") which is outside the bounds of the current simulation.");
    }
    
    if (age < 0) {
        error(string(__FUNCTION__) + " specified a negative age when " +
              "drawing location (" + integerToString(row) + ", " + integerToString(column) + ").");
    }
    
    age = min(age, kMaxAge);
    window.setColor(colors[age]);
    if (age == 0) {
        window.fillRect(upperLeftX + column * cellDiameter + 1, upperLeftY + row * cellDiameter + 1,
                        cellDiameter - 2, cellDiameter - 2);
    } else {
        window.fillOval(upperLeftX + column * cellDiameter + 1, upperLeftY + row * cellDiameter + 1,
                        cellDiameter - 2, cellDiameter - 2);
    }
    ages[row][column] = age;
}

int LifeDisplay::scalePrimaryColor(int baseContribution, int age) const {
    const int maxContribution = 220;
    int remaining = maxContribution - baseContribution;
    return baseContribution + double(age) * remaining / kMaxAge;
}

void LifeDisplay::initializeColors() {
    colors.add("White"); // colors[0] is used for age 0, and is always white
    int baseColor[] = {
        randomInteger(0, 192), randomInteger(0, 192), randomInteger(0, 192)
    };
    
    for (int age = 1; age <= kMaxAge; age++) {
        ostringstream oss;
        oss << "#";
        for (int primary = 0; primary < 3; primary++) {
            int contribution = scalePrimaryColor(baseColor[primary], age);
            oss << setw(2) << setfill('0') << hex << contribution;
        }
        colors.add(oss.str());
    }
}

void LifeDisplay::computeGeometry() {
 	int hPixelsPerCell = window.getHeight()/numRows;
	int wPixelsPerCell = window.getWidth()/numColumns;
	cellDiameter = min(wPixelsPerCell, hPixelsPerCell);
    upperLeftX = (kDisplayWidth - numColumns * cellDiameter)/2;
    upperLeftY = (kDisplayHeight - numRows * cellDiameter)/2;
}

bool LifeDisplay::coordinateInRange(int row, int column) const {
    return row >= 0 && row < numRows && column >= 0 && column < numColumns;
}

void LifeDisplay::printBoard() {
    cout << windowTitle << endl;
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numColumns; ++j) {
            cout << setw(3) << setfill(' ') << ages[i][j];
        }
        cout << endl;
    }
}
