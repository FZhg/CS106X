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
#include "gtimer.h" // for Gtimer constructor
#include "vector.h" // for the neighbors holder

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

/* Global constants */
const int N_ROWS = 45; // the number of row for setAutoColony fucntion.
const int N_COLS = 45; // the number of row for setAutoColony fucntion.
const float P_SEED = 0.5; // the probability that the specific grid will be seeded.
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
void setUserColony(ifstream  & colonyStream, Grid<int> & colony);
void drawBoard(LifeDisplay & display, const Grid<int> & grid);\
static void runAminimation(LifeDisplay& display, Grid<int>& board, int ms);
Grid<int> advanceBoard(const Grid<int>& board);
Vector<int> getNeighbors(const Grid<int>& board, int nRow, int nCol);
int getNumNeighbors(const Vector<int>& neighbors);
int getAdvanceAge(const Vector<int>& neighbors);
string getAnswer(string question);
int chooseMode();
static void manualMode(LifeDisplay& display, Grid<int>& colony);

/**
 * Function: manualMode
 * --------------------
 * @param display
 * @param colony
 * The manual mode: update the colony when the user hit RETURN.
 */
static void manualMode(LifeDisplay &display, Grid<int>& colony) {
    while(true) {
        string temp = getLine("Hit RETURN to continue (or type \"quit\" to end the simulation)");
        if (temp ==  "quit") break;
        drawBoard(display, colony);
        colony = advanceBoard(colony);
    }
}

/**
 * Function: chooseMode
 * --------------------
 * @return an interger that represents the choice
 */
int chooseMode() {
    int choice;
    cout << "You choose how fast to run the simulation." << endl;
    cout << "   1 = As fast as this chip can go!" << endl;
    cout << "   2 = Not too fast; this is a school zone." << endl;
    cout << "   3 = Nice and slow so I can watch everything that happens." << endl;
    cout << "   4 = wait for user to hit enter bettween generations." << endl;

    string response = getLine("Your Choice: ");
    while (response != "1" && response != "2" && response != "3"
           && response != "4") {
        cout << "Please enter your choice bettween 1 and 4." << endl;
        response = getLine("Your Choice: ");
    }
    choice = stringToInteger(response);
    if (choice != 4) {
        cout << "click and hold the mouse button to end the stimulation." << endl;
    }
    return choice;
}

/**
 * Function: getAnswer
 * @param question
 * @return a "yes" or "no"
 */
string getAnswer(string question) {
    string answer = getLine(question);
    while(answer != "yes" && answer != "no") {
        cout << "Please answer yes or no." << endl;
        getAnswer(question);
    }
    return answer;
}

/**
 * Function getAdvanceAge
 * @param neighbors
 * @return the correct age according to the rules
 */
int getAdvanceAge(const Vector<int>& neighbors) {
    int age = 0; // nonsense initial
    switch (getNumNeighbors(neighbors)) {
    case 0:
    case 1:
        age = 0;
        break;
    case 2:
        break;
    case 3:
        age ++;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        age = 0;
        break;
    }
    return age;
}

/**
 * Function: getNumNeighbors
 * ---------------------------
 * @param neighbors
 * @return  the number of neighbors
 */
int getNumNeighbors(const Vector<int>& neighbors) {
    int numNeighbors = 0;
    for (int i = 0; i < neighbors.size(); i ++) {
        if(neighbors[i] != 0) numNeighbors ++;
    }
    return numNeighbors;
}


/**
 * Function: getNeighbors
 * -----------------------
 * @param board
 * @param nRow: the row number of the bacteria which we want find neighbors with.
 * @param nCol: the columns number of the bacteria which we want find neighbors with.
 * @return an intergers vector by 8 * 1 holding neighbors bny order, the normal order;
 */
Vector<int> getNeighbors(const Grid<int>& board, int nRow, int nCol) {
    const int maxRow = board.height() - 1;
    const int maxCol = board.width() - 1;
    const int minRow = 0;
    const int minCol = 0;
    Vector<int> neighbors;

    for (int i = -1; i < 2; i ++) {
        for (int j = -1; j < 2; j ++) {
            if(i == 0 && j == 0) {
                continue; // you are not your neighbors
            } else {
                int age;
                int neighborRow = nRow + i;
                int neighborCol = nCol + j;
                if (neighborCol < minCol || neighborCol > maxCol ||
                        neighborRow < minRow || neighborRow > maxRow) {
                    age = 0; // error check for boundaries
                } else age = board.get(neighborRow, neighborCol);
                neighbors.add(age);
            }
        }
    }
    return neighbors;
}

/**
 * Function: advanceBoard
 * -----------------------
 * @param board
 * @return the new board
 */
Grid<int> advanceBoard(const Grid<int>& board) {
    Grid<int> futureBoard(board.height(), board.width());
    for(int i = 0; i < board.height(); i ++) {
        for (int j = 0; j < board.width(); j ++){
            futureBoard.set(i, j, getAdvanceAge(
                                getNeighbors(board, i, j)));
        }
    }
    return futureBoard;
}

/**
 * Function: runAminimation
 * -------------------------
 * @param display
 * @param board
 * @param ms: in milliseconds, the fram time, or speed chosen by the user;
 * This method is called when the user desn't choose the manual mode.
 * When the colony is stable or the user click the window, the stimulation will end.
 */
static void runAminimation(LifeDisplay &display, Grid<int> &board, int ms) {
    GTimer timer(ms);
    timer.start();
    while(true) {
        GEvent event = waitForEvent(TIMER_EVENT + MOUSE_EVENT);
        if (event.getEventClass() == TIMER_EVENT) {
            drawBoard(display, board);
            Grid<int> futureBoard = advanceBoard(board);
            if (futureBoard == board) {
                break; // has stabled
            } else {
                board = futureBoard; // iterate again
            }
        } else if (event.getEventType() == MOUSE_PRESSED) {
            break;
        }
    }
    timer.stop();
}

/**
 * Function: drawBoard
 * @param display; the LifeDisplay object to display the board.
 * @param grid; the grid that contains the position and the age of the bacterias.
 */
void drawBoard(LifeDisplay &  display, const Grid<int> & grid) {
    display.setDimensions(grid.height(),  grid.width());
    for(int i = 0; i < grid.height(); i ++) {
        for (int j = 0; j < grid.width(); j++) {
            display.drawCellAt(i, j, grid.get(i, j));
        }
    }
}

/**
 * Function: setUserColony
 * @param colonyStream: ifstream object holder.
 * @param colony: the grid object for unseeded colony
 * @return a seeded colony object that corresponds the user's configure file.
 */
void setUserColony(ifstream & colonyStream, Grid<int> & colony) {
    for(int i = 0; i <  colony.height(); i ++) {
        string line;
        getline(colonyStream, line);
        for (int j = 0; j < colony.width(); j++)
            if(line[j] == 'X') colony.set(i, j, 1);
    }
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

    setUserColony(colonyStream, colony);
    return colony;
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
    cout << "Ok, I will seed your colony randomly." << endl;
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
        cout << "screw \n" << endl;
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

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");

    setConsoleFont("Arial14");
    setConsoleLocation(20 , 600);
    setConsoleSize(720, 300);
   welcome();
   Grid<int> colony = seedColony();
   int choice = chooseMode();
   switch (choice) {
   case 1:
       runAminimation(display, colony, 1000);
       break;
   case 2:
       runAminimation(display, colony, 1500);
       break;
   case 3:
       runAminimation(display, colony, 2000);
       break;
   case 4:
       manualMode(display, colony);
   }


   //another around ?
   string answer = getAnswer("Would you like another simulation ?");
   if (answer == "yes") {
       return Main();
   } else {
       display.~LifeDisplay();
   }
    return 0;
}
