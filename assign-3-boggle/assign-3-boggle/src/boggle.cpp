/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */
 
#include <cctype>
#include <iostream>
#include<string>
#include <fstream>
#include "console.h"
#include "grid.h"
#include "gboggle.h"
#include "simpio.h"
#include "lexicon.h"
#include "vector.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include "set.h"
using namespace std;

static const int kBoggleWindowWidth = 650;
static const int kBoggleWindowHeight = 350;
static double hightlightPause = 800;

static const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

static const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};
const Lexicon EnglishLexicon("dictionary.txt");
static const int kMinLength = 4;
static const double kHighlightDelay = 350;

/*Functions Prototypes */

/**
 * Function: throwDices
 * Usage: throwDices(cubes)
 * -----------------
 * shuffle the cubes and randomsly choose a letter for every cube
 */
string throwDices(const string cubes[], unsigned int sizeCubes);

/**
 * @brief stringArray2Vector: convert a stirng array to a vector
 * @param arr: the string array
 * @return: the string vector converted
 */
Vector<string> stringArray2Vector(const string arr[], unsigned int sizeArr);

/**
 * @brief labelCubes: label each cube
 * @param cubesChosen: a string each character is the throwed cubes facing up
 */
static void labelCubes(string cubesChosen);

/**
 * @brief askCubes: ask the player to give customized cubes or use the stand
 * -ard boggle, big boggle
 * draw the the asked boggle on the board
 * @return a string for the final result
 */
string  askBoggle();

/**
 * @brief changeCube
 * @param cube
 * @return a string array of the cubes
 */
string changeCube(const string defaultCubes[], unsigned int sizeCubes);

/**
 * @brief isWordWrong
 * @param word: the new guess
 * @param words: the words that is already on the list
 * @return true if the word is wrong answer
 *         false if not
 */

/**
 * @brief humanTurn
 * @param boggle: the boggle combination
 * @param words: the words list that the player has already input correctly
 * @return all words that is guessed right by the user
 */
Set<string> humanTurn(string boggle, Set<string> & words);

/**
 * @brief findPath: find the path that forms the word
 * @param word: the desitination word to be formed
 * @param boggle: the same as before
 * @return all positions that form the word
 */
Vector<Vector<int>> findPath(string word, string boggle);

/**
 * @brief oneNodePath：find the path from start node
 * @param word: the destination word
 * @param positions: the current start position
 * @param board: the grid object that represent the boggle board
 * @return the path, which is vector of vectors of two ints, row idnex and column index
 */
Vector<Vector<int>> oneNodePath(string word, Vector<Vector<int>> positions, Grid<char> & board);

/**
 * @brief hightlightWord
 * @param positions: the path we have found
 */
static void hightlightWord(Vector<Vector<int>> & positions);

/**
 * @brief computerTurn: find all words on the board, and record for the computer
 * @param boggle: the boggle string
 * @return all words on the boggle board
 */
Set<string> computerTurn(string boggle);

/**
 * @brief computerFindWords found the all words along the path
 * @param r: the starting row index
 * @param c: the starting column index
 * @param computerWordList: the words that is found right now
 * @param board: the boggle board grid object
 * @return a vector of words that can be generated on the boggle board
 */
Set<string> computerFindWords(int r, int c, string path, Set<Vector<int>> visitedPositions, Set<string> & computerWordList, Grid<char> & board);

/**
 * @brief isVisited check the letter is used or not
 * @param positions: the already visited positions
 * @param position: the current positions
 * @return true if the position is already visited false if not
 */
bool isVisited(Vector<Vector<int>> & positions, Vector<int>  & position);
/** test functions **/
static void testThrowDices();
static void testHumanTurn();
static void testComputerTurn();
/**
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */
static void welcome() {
    setConsoleFont("newtimes-bold-18");
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl;
    cout << endl;
}

/**
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */
static void giveInstructions() {
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl;
    cout << endl;
    cout << "Hit return when you're ready...";
    getLine(); // ignore return value
}

/**
 * Function: playBoggle
 * --------------------
 * Manages all details needed for the user to play one
 * or more games of Boggle.
 */
static void playBoggle() {
    // set up board
    string boggle = askBoggle();
    Set<string> humanWords {};
    humanWords = humanTurn(boggle, humanWords);
    Set<string> computerWords;
    computerWords = computerTurn(boggle);
    if(computerWords.size() > humanWords.size()){
        cout << "Sorry! But the computer won." << endl;
    } else if(computerWords.size() == humanWords.size()){
        cout << "You have a tie with the computer." << endl;
    } else {
        cout << "Unbelievably, You won!" << endl;
    }
}

/**
 * Function: main
 * --------------
 * Serves as the entry point to the entire program.
 */
int main() {
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    initGBoggle(gw);
    welcome();
    if (getYesOrNo("Do you need instructions?")) giveInstructions();
    playBoggle();
    while (getYesOrNo("Would you like to play again? ")) {
        playBoggle();
    }
    return 0;
}

string throwDices(const string cubes[], unsigned int sizeCubes){
    /* copy */
    Vector<string> cubesCopy;
    cubesCopy = stringArray2Vector(cubes, sizeCubes);
    srand(time(NULL));
    /* shuffle */
    for (int i = 0; i < cubesCopy.size() - 1; i++){
         int r = rand() % (cubesCopy.size() - 1 - i) + i; /* i ~ cubeCupy.size() -1 */
         string temp = cubesCopy[i];
         cubesCopy[i] = cubesCopy[r];
         cubesCopy[r] = temp; /*swithch the r and i elements*/
    }
    /* choose */
    string cubesChosen = "";
    for (int i = 0; i < cubesCopy.size(); i++) {
        string cube = cubesCopy[i];
        int index = rand() % (cube.length() - 1);
        cubesChosen += cube[index];
    }
    return cubesChosen;
}

Vector<string> stringArray2Vector(const string arr[], unsigned int sizeArr){
    Vector<string> vec; // an naive implementation
    for (int i = 0; i < sizeArr; i++){
        vec.add(arr[i]);
    }
    return vec;
}

static void labelCubes(string cubesChosen){
    unsigned int boardSize = sqrt(cubesChosen.length());
    for (int r = 0; r < boardSize; r ++){
        for (int c =0; c < boardSize; c ++){
            labelCube(r, c, cubesChosen[r*boardSize + c]);
        }
    }
}

string askBoggle(){
    cout << "You can choose standard Boggle(4x4 grid)" << endl;
    cout << "or Big Boggle(5x5)" << endl;
    string boggle;
    if (getYesOrNo("Would you like Bigger Boggle?")){
        drawBoard(5, 5);
        boggle = changeCube(kBigBoggleCubes, 25);
    } else {
        drawBoard(4, 4);
        boggle = changeCube(kStandardCubes, 16);
    }
    return boggle;
}

string changeCube(const string defaultCubes[], unsigned int sizeCubes){
    unsigned int numRows = sqrt(sizeCubes);
    string cubesChanged;
    cout << "I'll give you a chance to set up the board to your specification, which makes";
    cout<< " it easier to confirm your boggle program is working." << endl;
    if (getYesOrNo("Do you want to force the board configuration? ")){
        cout <<"Enter a " << sizeCubes << "-character string to identify which letters you want on the cubes."<<endl;
        cout << "The first " << numRows << " letters are the cubes on the top row from left to right, the next ";
        cout << numRows << " letters are the second row, and so on." << endl;
        cubesChanged = getLine("Enter the string:");
        while(cubesChanged.length() < sizeCubes){
           cubesChanged = getLine("String must include " +to_string(sizeCubes) + " characters! Try again:");
        }
        cubesChanged = cubesChanged.substr(0, sizeCubes);
    } else{
        cubesChanged = throwDices(defaultCubes, sizeCubes);
    }
    cubesChanged = toUpperCase(cubesChanged);
    labelCubes(cubesChanged);
    return cubesChanged;
    }

Set<string> humanTurn(string boggle, Set<string> & words){
    string word = toUpperCase(getLine("Enter a word: "));
    if (word == "") {
        return words;
    }
    if (word.length() < 4){
        cout << "I'm sorry, but we have our standards." << endl;
        cout << "That word doesn't meet the minimum word length."<< endl;
        return humanTurn(boggle, words);
    } else if(!EnglishLexicon.contains(word)){
        cout << "That's not a word!" << endl;
        return humanTurn(boggle, words);
    } else if(words.contains(word)){
        cout <<"You've already guessed that!" << endl;
        return humanTurn(boggle, words);
    }
    Vector<Vector<int>> positions = findPath(word, boggle);
    if (positions.isEmpty()){
        cout << "You can't make that word!" << endl;
        return humanTurn(boggle, words);
    }
    words.add(word);
    hightlightWord(positions);
    recordWordForPlayer(word, HUMAN);
    return humanTurn(boggle, words);
}

Vector<Vector<int>> findPath(string word, string boggle){
    Vector<Vector<int>> positions;

    // build the board grid
    unsigned int numRows = sqrt(boggle.length());
    Grid<char> board(numRows, numRows);
    for (int r = 0; r < numRows; r ++){
        for(int c = 0; c < numRows; c++){
            board.set(r, c, boggle[r*numRows + c]);
        }
    }
    // check for every cell in the grid
    for (int r = 0; r < numRows; r ++){
        for(int c = 0; c < numRows; c++){
            if(word[0] == board.get(r, c)){
                Vector<int> newPosition;
                newPosition.add(r);
                newPosition.add(c); // never use plural and singular form to name variables again !!
                positions.add(newPosition);
                positions = oneNodePath(word, positions, board);
                if(positions.size() != word.length()){
                    positions.clear();
                }
                if(!positions.isEmpty()){
                    return positions;
                }
            }
        }
    }    
    return positions;
}

Vector<Vector<int>> oneNodePath(string word, Vector<Vector<int>> positions, Grid<char> & board){
    Vector<int> currentPosition = positions[positions.size() - 1];
    int r = currentPosition[0];
    int c = currentPosition[1];
    char nextLetter = word.at(positions.size());
    for(int deltaR = -1; deltaR < 2; deltaR ++){
        for(int deltaC = -1; deltaC < 2; deltaC ++){
            if((deltaR == 0) && (deltaC == 0)){
                continue; // skip the current position
            } else{
                if (((r+deltaR)>= 0) && ((r+deltaR)<=board.numRows() - 1) && ((c+deltaC) <= board.numCols() - 1) && ((c + deltaC) >= 0)){
                        int newR = r + deltaR;
                        int newC = c + deltaC;
                        Vector<int> newPosition;
                        newPosition.add(newR);
                        newPosition.add(newC);
                        if((board.get(newR, newC) == nextLetter) && (!isVisited(positions, newPosition))){
                            Vector<Vector<int>> newPositions = positions;
                            newPositions.add(newPosition);
                            if(newPositions.size() == word.length()){
                                return newPositions;
                            } else{
                                newPositions = oneNodePath(word, newPositions, board);
                                if(newPositions.size() == word.length()){
                                    return  newPositions;
                                }
                            }
                        }
                }
            }
        }
    }
    return positions;
}

static void hightlightWord(Vector<Vector<int>> & positions){
    for(int i = 0; i < positions.size(); i ++){
        Vector<int> position = positions[i];
        unsigned int r = position[0];
        unsigned int c = position[1];
        highlightCube(r, c, true);
    }
    pause(hightlightPause);
    for(int i = 0; i < positions.size(); i ++){
        Vector<int> position = positions[i];
        unsigned int r = position[0];
        unsigned int c = position[1];
        highlightCube(r, c, false);
    }
}

Set<string> computerTurn(string boggle){
    Set<string> computerWordsList {};

    // build a board
    unsigned int boardSize = sqrt(boggle.length());
    Grid<char> board(boardSize, boardSize);
    for (int r = 0; r < boardSize; r ++){
        for(int c = 0; c < boardSize; c ++){
            board.set(r, c, boggle[r * boardSize + c]);
        }
    }

    // try from every letter on the boggle board
    for (int r = 0; r < boardSize; r ++){
        for(int c =0; c < boardSize; c ++){
            string path = "";
            path += board.get(r, c);
            Vector<int> position;
            position.add(r);
            position.add(c);
            Set<Vector<int>> visitedPositions {position};
            computerWordsList = computerFindWords(r, c, path, visitedPositions,computerWordsList, board);

        }
    }
    return computerWordsList;
}

Set<string> computerFindWords(int r, int c, string path, Set<Vector<int>> visitedPositions, Set<string> & computerWordList, Grid<char> & board){
    for(int deltaR = -1; deltaR < 2; deltaR ++){
        for(int deltaC = -1; deltaC < 2; deltaC ++){
            int newR = r + deltaR;
            int newC = c + deltaC;
            if((newR <= board.numRows() - 1) && (newR >= 0) && (newC <= board.numCols() - 1) && (newC >= 0)){
                Vector<int> position;
                position.add(newR);
                position.add(newC);
                if(!visitedPositions.contains(position)){
                    string newPath = path + board.get(newR, newC);
                    Set<Vector<int>> newVisitedPositions = visitedPositions;
                    newVisitedPositions.add(position);
                    if(newPath.length() >= 4){
                        if(!computerWordList.contains(newPath)){
                            if(EnglishLexicon.contains(newPath)){
                                computerWordList.add(newPath);
                                recordWordForPlayer(newPath, COMPUTER);
                            }
                        }
                    }
                    if(EnglishLexicon.containsPrefix(newPath)){
                        computerWordList = computerFindWords(newR, newC, newPath, newVisitedPositions, computerWordList,  board);
                    }
                }
            }
        }
    }
    return computerWordList;
}

bool isVisited(Vector<Vector<int>> & positions, Vector<int> & position){
    for(int i = 0; i < positions.size(); i++){
        Vector<int> oldPosition = positions[i];
        if(oldPosition == position){
            return true;
        }
    }
    return false;
}

/** test functions **/
static void testThrowDices(){
    cout << sizeof(kStandardCubes) << endl;
    cout << sizeof(kStandardCubes[0]) << endl;
    unsigned int sizeKStandardCubes = sizeof(kStandardCubes) / sizeof(kStandardCubes[0]);
    cout << throwDices(kStandardCubes, sizeKStandardCubes) << endl;
}

static void testHumanTurn(){
    drawBoard(4, 4);
    string boggle = "LERAETIETOIIRSPN";
    labelCubes(boggle);
    Set<string> words {};
    cout << humanTurn(boggle, words) << endl;
}

static void testComputerTurn(){
    drawBoard(4, 4);
    string boggle = "LERAETIETOIIRSPN";
    boggle = toUpperCase(boggle);
    labelCubes(boggle);
    cout << computerTurn(boggle) <<endl;
}
