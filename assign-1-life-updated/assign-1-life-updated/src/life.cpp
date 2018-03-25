/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream>   // for ifstream
#include <cctype> //for isdigit
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"
#include "random.h" // for generate the random colony

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

/* Global constants */
const int N_ROWS = 100; // the number of row for setAutoColony fucntion.
const int N_COLS = 100; // the number of row for setAutoColony fucntion.
const float P_SEED = 0.4; // the probability that the specific grid will be seeded.
const int CONSOLE_WIDTH = 800;
const int CONSOLE_HEIGHT = 300;

/* Functions prototypes */
static void welcome();
bool isSetAutoColony();
Grid<int> seedColony();
Grid<int> setAutoColony(int nRows, int nCols, float p);
Grid<int> loadColony();
string askColonyFile();
string promptUserForFile(ifstream & infile, string prompt);
int getNum(ifstream & colonyStream);
Grid<int> setUserColony(ifstream colonyStream, Grid<int> & colony);

void test_isSetAutoColony(bool temp);
void test_setAutoColony();
void test_askColonyFile();
void test_loadColony();

/**
 * Function: setUserColony
 * @param colonyStream: ifstream object holder.
 * @param colony: the grid object for unseeded colony
 * @return a seeded colony object that corresponds the user's configure file.
 */
Grid<int> setUserColony(ifstream & colonyStream, Grid<int> & colony) {
    for(int i = 0; i <  colony.height(); i ++) {
        string line;
        getline(colonyStream, line);
        for (int j = 0; j < colony.width(); j++)
            if(line[j] == 'X') colony.set(i, j, 1);
    }

    return colony;
}
/**
 * Function: getNum
 * -----------------
 * @param colonyStream
 * @return the first number line the stream points to
 * Usage: so we can read number line one by one.
 */
int getNum(ifstream & colonyStream) {
    while(! colonyStream.eof()) {
       string line;
       getline(colonyStream, line);
       if(isdigit(line[0])) return stringToInteger(line);
    }
}

/**
 * Function: loadColony
 * @return the seeded colony corresponding the user's colony configure file.
 */
Grid<int> loadColony() {
    string colonyFilename = askColonyFile();
    ifstream colonyStream;
    colonyStream.open(colonyFilename.c_str());

    int nRows = getNum(colonyStream);
    int nCols = getNum(colonyStream); // since the configure format is error-proof

    Grid<int> colony(nRows, nCols);

    return setUserColony(colonyStream, colony);
}

/**
 * Function: askColonyFile
 * -----------------------
 * @return a egligible filename for post processing.
 */
string askColonyFile() {
    ifstream colonyStream;
    return promptUserForFile(colonyStream, "Please enter the filename:");
}

/**
 * Function: promptUserForFile
 * ---------------------------
 * @param infile: the ifstream holder for the filename
 * @param prompt: the string that ask for the filename path
 * @return an egligible filename which is able to be opened to be ifstream.
 */
string promptUserForFile(ifstream &infile, string prompt) {
    while(true) {
        cout << prompt;
        string filename;
        getline(cin, filename);
        infile.open(filename);
        if (! infile.fail()) return filename;
        infile.clear();
        cout << "Unable to open file named" << filename << ".Please try again." << endl;
        if(prompt == "") prompt = "Input file: ";
    }
}
/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

Grid<int> setAutoColony(int nRows, int nCols, float p) {
    Grid<int> colony(nRows, nCols);
    for(int i = 1; i < nRows; i ++) {
        for(int j = 1; j < nCols; j ++) {
            if(randomChance(p)) {
                colony.set(i, j, randomInteger(1, kMaxAge));  //the initial age may not be 1.
            }
        }
    }
    return colony;
}

/**
 * Funciton: seedColony
 * --------------------
 * @return the seeded colony as the use chooses.
 */
Grid<int> seedColony(){
    bool seedChoiceAuto = isSetAutoColony();
    if(seedChoiceAuto) {
        return setAutoColony(N_ROWS, N_COLS, P_SEED);
    } else {
        return loadColony();
    }
}

/**
 * Function: isSetAutoColony
 * -------------------------
 * @return true is the user want to configure the colony, false is the user doesn't.
 */
bool isSetAutoColony() {
    cout << "You can start your colony with random cells or read from a prepared ifle." << endl;
    string answer = getLine("Do you have a starting file in mind?");

    //error check for answer
    while(answer != "yes" && answer != "no") {
        cout << "Please answer yes or no." << endl;
        answer = getLine();
    }

    if (answer == "yes") {
        return false;
    } else {
        return true;
    }
}

/* tests for functions */
void test_isSetAutoColony(bool temp) {
    if (!temp) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
}

void test_setAutoColony() {
    Grid<int> colony = setAutoColony(N_ROWS,N_COLS, P_SEED);
    cout << colony.toString2D() << endl;
}

void test_askColonyFile() {
    string colonyFilename = askColonyFile();
    ifstream colonyStream;
    colonyStream.open(colonyFilename);
    string line;
    while(!colonyStream.eof()) {
        getline(colonyStream, line);
        cout << line << endl;
    }
}

void test_loadColony() {
    Grid<int> colony = loadColony();
    cout << colony.toString2D() << endl;
}

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");
    welcome();

    // tests, uncomments to run  test
    // test_isSetAutoColony(isSetAutoColony());
    // test_setAutoColony();
    // test_askColonyFile();
    test_loadColony();
    return 0;
}
