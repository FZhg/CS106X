/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"
#include "maze-graphics.h"
#include "maze-types.h"
#include "set.h"
#include "foreach.h"
#include <algorithm>
#include <random>
#include <chrono>

static int getMazeDimension(string prompt, int minDimension, int maxDimension);

/**
 * @brief generateCells
 * @param demension
 * @return a set of cell that contains all dem*dem cells
 */
Vector<cell> generateCells(int dimension);

/**
 * @brief generateWalls
 * @param dimension
 * @return a set of walls
 */
Vector<wall> generateWalls(int dimension);

/**
 * @brief shuffleWalls
 * @param walls
 * @return a set of randomed ordered walls
 */
Vector<wall> shuffleWalls(const Vector<wall>& walls);

/**
 * @brief isOneChamber
 * @param cell1
 * @param cell2
 * @return true if the cell1 and cell2 is in the same chamber
 */
bool isOneChamber(cell& cell1, cell& cell2, Set<Set<cell>>& chambers);

/**
 * @brief UpdateChambers
 * @param cell1
 * @param cell2
 * @param chambers
 * update the chambers usually by linking chambers that contains either cell1 or cell2
 */
void updateChambers(cell& cell1, cell& cell2, Set<Set<cell>>& chambers);

/* test for method */
void testGenerateCells();
void testGenerateWalls();
void showcell(cell chamber);
void testShuffleWalls();
void showWalls(Vector<wall> walls);
void showcells(Vector<cell> cells);
void showcells(Set<cell> cells);
void showChambers(Set<Set<cell>> chambers);

static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
	while (true) {
		int response = getInteger(prompt);
		if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
		cout << "Please enter a number between "
			 << minDimension << " and " 
		     << maxDimension << ", inclusive." << endl;
	}
}

Vector<wall> generateWalls(int dimension) {
    Vector<wall> walls;
    for(int i = 0; i < dimension; i ++) {
        for(int j = 0; j < dimension; j ++) {
            cell center, down, right;
            center.row = i, center.col = j;
            down.row = i + 1, down.col = j;
            right.row = i, right.col = j + 1;

            wall horizotal;
            wall vertical;
            horizotal.one = center;
            horizotal.two = down;
            vertical.one = center;
            vertical.two = right;

            // to elilminate the borders
            if (i != dimension - 1) {
                 walls.add(horizotal);
            }

            if(j != dimension - 1) {
                walls.add(vertical);
            }

        }
    }
    return walls;
}

Vector<cell> generateCells(int dimension) {
    Vector<cell> cells;
    for(int i = 0; i < dimension; i ++) {
        for(int j = 0; j < dimension; j ++) {
            cell ijcell;
            ijcell.row = i;
            ijcell.col = j;
            cells.add(ijcell);
        }
    }
    return cells;
}

Vector<wall> shuffleWalls(const Vector<wall>& walls) {
    Vector<wall> randomWalls = walls;

    // obtain a time-based seed
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(randomWalls.begin(), randomWalls.end(), default_random_engine(seed));
    return randomWalls;

}

bool isOneChamber(cell &cell1, cell &cell2, Set<Set<cell>>& chambers) {
    foreach (Set<cell> chamber in chambers) {
        if (chamber.contains(cell1) && chamber.contains(cell2)) {
            return true;
        }
    }
    return false;
}

void updateChambers(cell &cell1, cell &cell2, Set<Set<cell> >& chambers) {
    Set<cell> merged;
    foreach (Set<cell> chamber in chambers) {
        if(chamber.contains(cell1) || chamber.contains(cell2)) {
            merged += chamber;
            chambers.remove(chamber);
        }
    }
    chambers.add(merged);
}


/* test  for methods */
void testGenerateCells() {
    Vector<cell> testResult = generateCells(2);
    showcells(testResult);
}

void testGenerateWalls() {
    Vector<wall> testResult = generateWalls(2);
    showWalls(testResult);
}

void showcell(cell chamber) {
     cout << " the cell is located at (" << chamber.row << " ," << chamber.col << ")."<<endl;
}

void testShuffleWalls() {
    Vector<wall> testCase = generateWalls(3);
    showWalls(testCase);

    cout << "Shuffle the walls" << endl;

    Vector<wall> testResult = shuffleWalls(testCase);
    showWalls(testResult);
}

void showWalls(Vector<wall> walls) {
    foreach (wall ijwall in walls) {
        cell cell1 = ijwall.one;
        cell cell2 = ijwall.two;
        showcell(cell1);
        showcell(cell2);
    }
}

void showcells(Vector<cell> cells) {
    foreach (cell ijcell in cells) {
        showcell(ijcell);
    }
}

void showcells(Set<cell> cells) {
    foreach (cell ijcell in cells) {
        showcell(ijcell);
    }
}

void showChambers(Set<Set<cell> > chambers) {
    int i = 0;
    foreach (Set<cell> chamber in chambers) {
        i ++;
        cout << " **************** chamber " << i << " **************" << endl;
        showcells(chamber);
    }
}

int main() {
    setConsoleFont("Times-Bold-24");
    setConsoleSize(800, 200);
    setConsoleLocation(1100, 100);
     MazeGeneratorView maze;
	while (true) {
		int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
		if (dimension == 0) break;

        // generate all chambers
        Vector<cell> cells = generateCells(dimension);
        // generate the walls
        Vector<wall> walls = generateWalls(dimension);

        // draw the walls
        maze.setDimension(dimension);
        maze.drawBorder();
        foreach (wall ijwall in walls) {
            maze.drawWall(ijwall);
        }

        // shuffle the walls
        Vector<wall> randomWalls = shuffleWalls(walls);

        Set<Set<cell>> chambers;
        foreach (cell ijcell in cells) {
            Set<cell> chamber;
            chamber.add(ijcell);
            chambers.add(chamber);
        } // initialize the chambers

        foreach (wall ijwall in randomWalls) {
            cell cell1 = ijwall.one;
            cell cell2 = ijwall.two;
            if(!isOneChamber(cell1, cell2, chambers)) {
                maze.removeWall(ijwall);
                updateChambers(cell1, cell2, chambers);
            }
        }

	}


	return 0;
}
